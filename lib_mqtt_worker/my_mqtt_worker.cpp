//
// Created by vovan on 07.04.2020.
//

#include <zconf.h>
#include "my_mqtt_worker.h"

MyMQTTWorker::MyMQTTWorker(std::string address, std::string clientID, std::string topic, int QOS) {
    this->address = address;
    this->clientID = clientID;
    this->topic = topic;
    this->QOS = QOS;

    this->isWorking = false;
    this->isSendingData = false;
}

void MyMQTTWorker::enableMQTTServer() {
    if (!isWorking) {
        isWorking = true;
        this->mqttServerThread = std::thread([this]() {
            mqtt::async_client cli(address, clientID, MAX_BUFFERED_MSGS, PERSIST_DIR);

            mqtt::connect_options connOpts;
            connOpts.set_keep_alive_interval(MAX_BUFFERED_MSGS * PERIOD);
            connOpts.set_clean_session(true);
            connOpts.set_automatic_reconnect(true);

            mqtt::topic top(cli, topic, QOS, true);

            try {
                std::cout << "Connecting to server '" << address << "'..." << std::flush;
                cli.connect(connOpts)->wait();
                std::cout << "OK\n" << std::endl;
                int num = 0;
                while (isWorking) {
                    sleep(1);
                    if (isSendingData) {
                        v_lock.lock();
                        if (list_JSON_sensors_data.size() > 0) {
                            std::string message = *list_JSON_sensors_data.begin();
                            list_JSON_sensors_data.pop_front();
                            std::cout << message << std::endl;
                            top.publish(std::move(message));
                        }
                        v_lock.unlock();
                    }
                }

                std::cout << "\nDisconnecting..." << std::flush;
                cli.disconnect()->wait();
                std::cout << "OK" << std::endl;
            }
            catch (const mqtt::exception& exc) {
                std::cerr << exc.what() << std::endl;
                std::cerr << "problems with mqtt-client's connection" << std::endl;
            }
        });
    }
}

void MyMQTTWorker::disableMQTTServer() {
    if (isWorking) {
        isWorking = false;
        isSendingData = false;
    }
}

void MyMQTTWorker::enableSendingData() {
    if (isWorking && !isSendingData) {
        isSendingData = true;
    }
}

void MyMQTTWorker::disableSendingData() {
    if (isWorking && isSendingData) {
        isSendingData = false;
    }
}

void MyMQTTWorker::addJSONData(std::string sensorsData) {
    v_lock.lock();
    list_JSON_sensors_data.push_back(sensorsData);
    if (list_JSON_sensors_data.size() > MAX_JSON_LIST_SIZE)
        list_JSON_sensors_data.pop_front();
    v_lock.unlock();
}

//
// Created by vovan on 07.04.2020.
//

#ifndef WS_DEVICES_2_MY_PAHO_WORKER_H
#define WS_DEVICES_2_MY_PAHO_WORKER_H

#define MAX_JSON_LIST_SIZE  20          //max size of list, used to control data using

#include <random>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "mqtt/async_client.h"

const auto PERIOD = std::chrono::seconds(5);
const int MAX_BUFFERED_MSGS = 120;	                    // 120 * 5sec => 10min off-line buffering
const std::string PERSIST_DIR { "data-persist" };

/**This is a class for connecting to MQTT and sending data to MQTT-subscribers
 * FIELDS for MQTT settings:
 * * string address - address of MQTT service
 * * string clientID - usually random name of client, used by MQTT-service
 * * string topic - MQTT topic for publications in MQTT service
 * * int QOS - quality of service, used for enabling different message quality
 *
 * MyMQTTWorker fields for thread settings:
 * * isWorking - status of MQTT-connecting and working a client - shows - is client thread works, or no.
 * * isSendingData - status of sending data to MQTT-topic
 * * thread mqttServerTread - thread for sending data to service
 * * list<std::string> list_JSON_sensors_data - list of data, contains actual data
 *
 * MyMQTTWorker functions:
 * * enableMQTTServer() - used to start MQTT sending thread
 * * disableMQTTServer() - used to stop MQTT sending thread //have some problems here, actually doesn't used
 * * enableSendingData() - used to turn on MQTT sending data, doesn't run thread
 * * disbleSendingData() - used to turn off MQTT sending data, doesn't stop thread
 * */
class MyMQTTWorker {
public:
    MyMQTTWorker(std::string address, std::string clientID, std::string topic, int QOS);
    void enableMQTTServer();
    void disableMQTTServer();
    void enableSendingData();
    void disableSendingData();

    void addJSONData(std::string recordOfSensorsData);

private:
    std::list<std::string> list_JSON_sensors_data;
    std::mutex v_lock;
    std::string address;
    std::string clientID;
    std::string topic;
    int QOS;

    bool isWorking;
    bool isSendingData;
    std::thread mqttServerThread;
};


#endif //WS_DEVICES_2_MY_PAHO_WORKER_H

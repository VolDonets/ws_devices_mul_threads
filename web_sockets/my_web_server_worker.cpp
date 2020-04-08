//
// Created by vovan on 07.04.2020.
//

#include <zconf.h>
#include "my_web_server_worker.h"


MyWebServerWorker::MyWebServerWorker() {
    this->isSendingData = false;
    this->is_have_urgent_message = false;
    this->startServer();
    sleep(1);
    this->processServer();
}

void MyWebServerWorker::startServer() {
    this->server_thread = std::thread([this]() {
        logger = std::make_shared<PrintfLogger>();
        ws_server = make_shared<MyServer>(logger);
        handler = std::make_shared<MyHandler>(ws_server.get());
        ws_server->addPageHandler(std::make_shared<MyAuthHandler>());
        ws_server->addWebSocketHandler("/chart", handler);
        ws_server->serve("src/server_files", 9090);
    });
}

void MyWebServerWorker::processServer() {
    this->worker_thread = std::thread([this](){
        while (1) {
            if(is_have_urgent_message) {
                this->handler->sendValuesJSON(urgent_message);
                is_have_urgent_message = false;
            }
            if (isSendingData) {
                v_lock.lock();
                if (list_JSON_sensors_data.size() > 0) {
                    std::string message = *list_JSON_sensors_data.begin();
                    list_JSON_sensors_data.pop_front();
                    this->handler->sendValuesJSON(message);
                }
                v_lock.unlock();
            }
            sleep(1);
        }
    });
}

void MyWebServerWorker::addJSONData(std::string recordOfSensorsData) {
    v_lock.lock();
    list_JSON_sensors_data.push_back(recordOfSensorsData);
    if (list_JSON_sensors_data.size() > MAX_JSON_LIST_SIZE)
        list_JSON_sensors_data.pop_front();
    v_lock.unlock();
}

void MyWebServerWorker::enableSending() {
    isSendingData = true;
}

void MyWebServerWorker::disableSending() {
    isSendingData = false;
}

void MyWebServerWorker::setUrgentMessage(std::string message) {
    urgent_message = message;
    is_have_urgent_message = true;
}

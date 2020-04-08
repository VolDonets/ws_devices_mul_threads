//
// Created by vovan on 07.04.2020.
//

#ifndef WS_DEVICES_2_MY_WEB_SERVER_WORKER_H
#define WS_DEVICES_2_MY_WEB_SERVER_WORKER_H

#define MAX_JSON_LIST_SIZE 20                //set a max list's size with data from sensors

#include <random>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>

//#include "mqtt/async_client.h"
#include "WebServer.h"

class MyWebServerWorker {
public:
    MyWebServerWorker();
    void addJSONData(std::string recordOfSensorsData);
    void setUrgentMessage(std::string message);
    void enableSending();
    void disableSending();

private:
    void startServer();
    void processServer();

    std::string urgent_message;
    bool is_have_urgent_message;
    std::list<std::string> list_JSON_sensors_data;
    std::mutex v_lock;
    bool isSendingData;
    std::thread server_thread;
    std::thread worker_thread;
    shared_ptr<PrintfLogger> logger;
    shared_ptr<MyServer> ws_server;
    shared_ptr<MyHandler> handler;
};


#endif //WS_DEVICES_2_MY_WEB_SERVER_WORKER_H

//
// Created by vovan on 25.03.2020.
//

#ifndef WS_DEVICES_2_PROCESSING_H
#define WS_DEVICES_2_PROCESSING_H

#include "lib_mpu6050/mpu6050_drive.h"
#include "web_sockets/WebServer.h"
#include "lib_bme280/bme280.h"

class Processing {
public:
    Processing(i2c_init_func_def, i2c_read_func_def, i2c_write_func_def, i2c_read_func_def, i2c_write_func_def);
    void start();

private:
    MPU6050_Drive *mpu6050_drive;
    BME280 *bme280_drive;
    thread server_thread;
    shared_ptr<PrintfLogger> logger;
    shared_ptr<MyServer> ws_server;
    shared_ptr<MyHandler> handler;

    void init_server();
    void processing_data_to_websocket();
    std::string to_json_process();
};


#endif //WS_DEVICES_2_PROCESSING_H

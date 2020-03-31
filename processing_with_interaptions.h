//
// Created by vovan on 30.03.2020.
//

#ifndef WS_DEVICES_2_PROCESSING_WITH_INTERAPTIONS_H
#define WS_DEVICES_2_PROCESSING_WITH_INTERAPTIONS_H

#include "lib_mpu6050/mpu6050_drive.h"
#include "web_sockets/WebServer.h"
#include "lib_bme280/bme280.h"
#include "lib_ssd1306/ssd1306_driver.h"

class ProcessingInter {
public:
    ProcessingInter(i2c_init_func_def, i2c_read_func_def, i2c_write_func_def, i2c_read_func_def, i2c_write_func_def, i2c_read_func_def, i2c_write_func_def);
    void start();

private:
    MPU6050_Drive *mpu6050_drive;
    BME280 *bme280_drive;
    SSD1306_Drive *ssd1306_drive;
    thread server_thread;
    shared_ptr<PrintfLogger> logger;
    shared_ptr<MyServer> ws_server;
    shared_ptr<MyHandler> handler;

    float temperature;
    int connections;


    void init_server();
    void processing_data_to_websocket();
    std::string to_json_process();
    void show_on_display();
    void read_temperature(); //not right function
};


#endif //WS_DEVICES_2_PROCESSING_WITH_INTERAPTIONS_H

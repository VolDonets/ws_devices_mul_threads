//
// Created by vovan on 30.03.2020.
//

#ifndef WS_DEVICES_2_PROCESSING_WITH_INTERRUPT_H
#define WS_DEVICES_2_PROCESSING_WITH_INTERRUPT_H

#include "lib_mpu6050/mpu6050_drive.h"
#include "web_sockets/WebServer.h"
#include "lib_bme280/bme280.h"
#include "lib_ssd1306/ssd1306_driver.h"
#include "web_sockets/lib_my_event_handler/handler_ws.h"
#include "web_sockets/lib_my_event_handler/event_ws.h"
#include "web_sockets/lib_my_event_handler/delegate_ws.h"
#include "lib_mqtt_worker/my_mqtt_worker.h"
#include "web_sockets/my_web_server_worker.h"

struct MyI2CDriveMethods {
    i2c_init_func_def i2c_init;
    i2c_read_func_def i2c_read_mpu;
    i2c_write_func_def i2c_write_mpu;
    i2c_read_func_def i2c_read_bme;
    i2c_write_func_def i2c_write_bme;
    i2c_read_func_def i2c_read_ssd;
    i2c_write_func_def i2c_write_ssd;
};

class ProcessingInter: public HandlerWS{
public:
    ProcessingInter(MyI2CDriveMethods&);
    void start();
    void handleEventWS(EventWS& event);

private:
    shared_ptr<MPU6050_Drive> mpu6050_drive;
    shared_ptr<BME280> bme280_drive;
    shared_ptr<SSD1306_Drive>ssd1306_drive;
    shared_ptr<MyMQTTWorker> myMQTTWorker;
    shared_ptr<MyWebServerWorker> myWebServerWorker;

    MPU6050Data *mpu6050Data;
    BMP280Data *bme280data;

    float temperature;
    int connections;
    bool isEnabledMQTT, isWorkingMQTT;


    void enable_MQTT_server();
    void disable_MQTT_server();
    std::string to_json_process();
    void show_on_display();
};


#endif //WS_DEVICES_2_PROCESSING_WITH_INTERRUPT_H

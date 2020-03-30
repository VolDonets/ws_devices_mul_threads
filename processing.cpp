//
// Created by vovan on 25.03.2020.
//

#include "processing.h"

/*Here constructor is a constructor of a class Processing
 * Here we create some objects for I2Driver and for MPU6050 Driver
 * for initialization and reading values from devices
 * ~>public constructor*/

Processing::Processing(i2c_init_func_def i2c_init, i2c_read_func_def i2c_read_mpu, i2c_write_func_def i2c_write_mpu,
                       i2c_read_func_def i2c_read_bme, i2c_write_func_def i2c_write_bme) {
    mpu6050_drive = new MPU6050_Drive(i2c_init, i2c_read_mpu, i2c_write_mpu);
    mpu6050_drive->init_mpu6050();

    bme280_drive = new BME280("dev/i2c-1/", BME280_ADDRESS, i2c_init, i2c_read_bme, i2c_write_bme);
    bme280_drive->defaultInit();
}

/*This function activate server thread and threads for different devices
 * ~>public function*/
void Processing::start() {
    init_server();      //it starts web_socket server
    while (1) {
        sleep(1);
        this->processing_data_to_websocket();
    }
}

/*This function configures a web-server and starts a server-thread
 * ~>private function*/
void Processing::init_server(){
   this->server_thread = thread([this]() {
       logger = std::make_shared<PrintfLogger>();
       ws_server = make_shared<MyServer>(logger);
       handler = std::make_shared<MyHandler>(ws_server.get());
       ws_server->addPageHandler(std::make_shared<MyAuthHandler>());
       ws_server->addWebSocketHandler("/chart", handler);
       ws_server->serve("src/server_files", 9090);
    });
   //this->server_thread.join();
}

/*This function processes data from different devices and sends it to server
 * ~>private function*/
void Processing::processing_data_to_websocket() {
    std:string values = to_json_process();
    handler->sendValuesJSON(values);
}

/*this function get data from connected devices and convert it to JSON for sending this to server and clients*
 * ~>private*/
std::string Processing::to_json_process() {
    float accelX, accelY, accelZ, gyroX, gyroY, gyroZ,
        temperature, pressure, humidity, altitude;
    accelX = mpu6050_drive->get_accel_X();
    accelY = mpu6050_drive->get_accel_Y();
    accelZ = mpu6050_drive->get_accel_Z();
    gyroX = mpu6050_drive->get_gyro_X();
    gyroY = mpu6050_drive->get_gyro_Y();
    gyroZ = mpu6050_drive->get_gyro_Z();

    BMP280Data *bme280_data = bme280_drive->getBMP280Data();
    temperature = bme280_data->getTemperature();
    pressure = bme280_data->getPressure();
    humidity = bme280_data->getHumidity();
    altitude = bme280_data->getAltitude();


    std::string result = "{\"accelX\": " + std::to_string(accelX)
            + ", \"accelY\": " + std::to_string(accelY)
            + ", \"accelZ\": " + std::to_string(accelZ)
            + ", \"gyroX\": " + std::to_string(gyroX)
            + ", \"gyroY\": " + std::to_string(gyroY)
            + ", \"gyroZ\": " + std::to_string(gyroZ)
            + ", \"temperature\": " + std::to_string(temperature)
            + ", \"pressure\": " + std::to_string(pressure)
            + ", \"humidity\": " + std::to_string(humidity)
            + ", \"altitude\": " + std::to_string(altitude)
            + "}";

    return result;
}
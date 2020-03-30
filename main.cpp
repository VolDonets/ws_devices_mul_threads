#include <iostream>

#include "processing.h"
#include "lib_i2c/i2c_driver.h"

I2C_Driver *i2c_driver = new I2C_Driver(I2C_ADAPTER_1);

//def function which is using in MPU6050_Drive, BME280, ...
int i2c_init();

//def functions which are using in MPU6050_Drive
int i2c_read_mpu(uint8_t reg, int read_len, uint8_t read_res[]);
int i2c_write_mpu(uint8_t reg, uint8_t data);

//def functions which are using in BME280;
int i2c_read_bme(uint8_t reg, int read_len, uint8_t read_res[]);
int i2c_write_bme(uint8_t reg, uint8_t data);

//start point of working ws-server thread and thread for reading values from devices
int main() {
    Processing *processing = new Processing(i2c_init, i2c_read_mpu, i2c_write_mpu,
            i2c_read_bme, i2c_write_bme);
    processing->start();
    return 0;
}

/*this function is raper for I2C_Driver for using in MPU6050Driver, BME280, ...*/
int i2c_init() {
    return i2c_driver->open_i2c_file();
}

/*this function is raper for I2C_Driver for using in MPU6050Driver*/
int i2c_read_mpu(uint8_t reg, int read_len, uint8_t read_res[]){
    return i2c_driver->read_buffer_i2c(Device_Address_mpu6050, reg, read_len, read_res);
}

/*this function is raper for I2C_Driver for using in MPU6050Driver*/
int i2c_write_mpu(uint8_t reg, uint8_t data) {
    return i2c_driver->write_buffer_i2c(Device_Address_mpu6050, reg, data);
}

/*this function is raper for I2C_Driver for using in BME280*/
int i2c_read_bme(uint8_t reg, int read_len, uint8_t read_res[]){
    return  i2c_driver->read_buffer_i2c(BME280_ADDRESS, reg, read_len, read_res);
}

/*this function is raper for I2C_Driver for using in BME280*/
int i2c_write_bme(uint8_t reg, uint8_t data) {
    return  i2c_driver->write_buffer_i2c(BME280_ADDRESS, reg, data);
}

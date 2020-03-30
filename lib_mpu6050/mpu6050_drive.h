//
// Created by vovan on 23.03.2020.
//

#ifndef MQTT_SS1306_BME280_MPU6050_MPU6050_DRIVE_H
#define MQTT_SS1306_BME280_MPU6050_MPU6050_DRIVE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <cstdint>

#define Device_Address_mpu6050 0x68	/*Device Address/Identifier for MPU6050*/

#define PWR_MGMT_1   0x6B
#define SMPLRT_DIV   0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define INT_ENABLE   0x38
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define GYRO_XOUT_H  0x43
#define GYRO_YOUT_H  0x45
#define GYRO_ZOUT_H  0x47

#define ACCEL_CONST 16384.0	/*const for getting accelerometer parameters in g. (grams)*/
#define GYRO_CONST 131.0	/*const for getting gyro paramenter in * (degree)*/

typedef int (*i2c_write_func_def)(uint8_t reg, uint8_t data);
typedef int (*i2c_read_func_def)(uint8_t reg, int read_len, uint8_t read_res[]);
typedef int (*i2c_init_func_def)();

class MPU6050_Drive {
private:
    i2c_init_func_def i2c_init;
    i2c_read_func_def i2c_read;
    i2c_write_func_def i2c_write;

    /*this function return value of accelerometer or gyroscope in grams or degrees
    *int reg - register for reading
    *float del_val - delimiter for getting values in grams or degrees*/
    float get_accel_gyro(int reg, float del_val);


public:
    MPU6050_Drive(i2c_init_func_def, i2c_read_func_def, i2c_write_func_def);

    /*full activation of mpu6050 without file value for other app*/
    /*PLEASE USE THIS ONE BEFORE YOU GET VALUES FROM MPU*/
    int init_mpu6050();

    /*this function for testing work of mpu6050, it just print concrete value of parameters*/
    void test_read_data_mpu6050();

    /*next functions for getting acceleleromener's and gyro's parameters*/
    float get_accel_X();
    float get_accel_Y();
    float get_accel_Z();
    float get_gyro_X();
    float get_gyro_Y();
    float get_gyro_Z();

    /*this function for fast read data from mpu6050 when smt happens
    *float values** - array of results of reading, where
		[][0] - is value accel by X
		[][1] - is value accel by Y
		[][2] - is value accel by Z

		[][3] - is value gyro by X
		[][4] - is value gyro by Y
		[][6] - is value gyro by Z
    *int lenght - lenght of array values and count of readet data*/
    void get_array_AG_val(float *values[6], int length);

    /*this function for convering into JSON format of data
    *char json_arr** - array of results of converting
    *float values** - array of results of reading, where
		[][0] - is value accel by X
		[][1] - is value accel by Y
		[][2] - is value accel by Z

		[][3] - is value gyro by X
		[][4] - is value gyro by Y
		[][6] - is value gyro by Z
    *int lenght - lenght of array values and count of readet data*/
    void to_JSON_array_AG_val(char *json_arr[], float *values[6], int length);

    /*function for deactivation mpu6050*/
    /*PLEASE USE THIS WHEN YOU TOTALLY END WORK WITH MPU6050*/
    void full_deactivate_mpu();
};


#endif //MQTT_SS1306_BME280_MPU6050_MPU6050_DRIVE_H

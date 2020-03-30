//
// Created by vovan on 23.03.2020.
//

#include "mpu6050_drive.h"

MPU6050_Drive::MPU6050_Drive(i2c_init_func_def i2c_init, i2c_read_func_def i2c_read, i2c_write_func_def i2c_write) {
    this->i2c_init = i2c_init;
    this->i2c_write = i2c_write;
    this->i2c_read = i2c_read;
}

/*this is function for full activation mpu6050 by i2c bus*/
int MPU6050_Drive::init_mpu6050() {
    if(i2c_init() < 0)
        return -1;
    i2c_write(SMPLRT_DIV, 0x07);				/* Write to sample rate register */
    i2c_write(PWR_MGMT_1, 0x01);				/* Write to power management register */
    i2c_write(CONFIG, 0);					    /* Write to Configuration register */
    i2c_write(GYRO_CONFIG, 24);			    	/* Write to Gyro Configuration register */
    return i2c_write(INT_ENABLE, 0x01);		    /* Write to interrupt enable register */
}

/*this function return value of accelerometer or gyroscope in grams or degrees
*int reg - register for reading
*float del_val - delimiter for getting values in grams or degrees*/
float MPU6050_Drive::get_accel_gyro(int reg, float del_val) {
    uint8_t arr[2];
    short cha0, cha1, cha;
    if (i2c_read(reg, 2, arr) >= 0) {
        cha0 = (short)arr[0];
        cha1 = (short)arr[1];
        cha = (cha0 << 8) | cha1;
        return ((float)cha) / del_val;
    }
    return 0.0;
}


/*this function for testing work of mpu6050, it just print concrete value of parameters*/
void MPU6050_Drive::test_read_data_mpu6050() {
    float Ax = 0, Ay = 0, Az = 0;
    float Gx = 0, Gy = 0, Gz = 0;

    Ax = get_accel_gyro(ACCEL_XOUT_H, ACCEL_CONST);
    Ay = get_accel_gyro(ACCEL_YOUT_H, ACCEL_CONST);
    Az = get_accel_gyro(ACCEL_ZOUT_H, ACCEL_CONST);

    Gx = get_accel_gyro(GYRO_XOUT_H, GYRO_CONST);
    Gy = get_accel_gyro(GYRO_YOUT_H, GYRO_CONST);
    Gz = get_accel_gyro(GYRO_ZOUT_H, GYRO_CONST);

    printf("\n Gx=%.3f °/s\tGy=%.3f °/s\tGz=%.3f °/s\tAx=%.3f g\tAy=%.3f g\tAz=%.3f g", Gx, Gy, Gz, Ax, Ay, Az);
}

/*next functions for getting acceleleromener's and gyro's parameters*/
float MPU6050_Drive::get_accel_X() {
    return get_accel_gyro(ACCEL_XOUT_H, ACCEL_CONST);
}
float MPU6050_Drive::get_accel_Y() {
    return get_accel_gyro(ACCEL_YOUT_H, ACCEL_CONST);
}
float MPU6050_Drive::get_accel_Z() {
    return get_accel_gyro(ACCEL_ZOUT_H, ACCEL_CONST);
}
float MPU6050_Drive::get_gyro_X() {
    return get_accel_gyro(GYRO_XOUT_H, GYRO_CONST);
}
float MPU6050_Drive::get_gyro_Y() {
    return get_accel_gyro(GYRO_YOUT_H, GYRO_CONST);
}
float MPU6050_Drive::get_gyro_Z() {
    return get_accel_gyro(GYRO_ZOUT_H, GYRO_CONST);
}

/*function for deactivation mpu6050*/
void MPU6050_Drive::full_deactivate_mpu() {

}


/*this function for fast read data from mpu6050 when smt happens
*float values** - array of results of reading, where
		[][0] - is value accel by X
		[][1] - is value accel by Y
		[][2] - is value accel by Z

		[][3] - is value gyro by X
		[][4] - is value gyro by Y
		[][6] - is value gyro by Z
*int lenght - lenght of array values and count of readet data*/
void MPU6050_Drive::get_array_AG_val(float *values[6], int length) {
    for (int i = 0; i < length; i++) {
        values[i][0] = get_accel_X();
        values[i][1] = get_accel_Y();
        values[i][2] = get_accel_Z();

        values[i][3] = get_gyro_X();
        values[i][4] = get_gyro_Y();
        values[i][5] = get_gyro_Z();
    }
}

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
void MPU6050_Drive::to_JSON_array_AG_val(char *json_arr[], float *values[6], int length) {
    for (int i = 0; i < length; i++) {
       char json_str[80];
        sprintf(json_str, "{\"accelX\": %.3f, \"accelY\": %.3f, \"accelZ\": %.3f, \"gyroX\": %.3f, \"gyroY\": %.3f, \"gyroZ\": %.3f}",
            values[i][0], values[i][1], values[i][2], values[i][3], values[i][4], values[i][5]);
        json_arr[i] = json_str;
    }
}
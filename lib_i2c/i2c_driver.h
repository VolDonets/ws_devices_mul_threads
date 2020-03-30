//
// Created by vovan on 23.03.2020.
//

#ifndef MY_BME280_DRIVE_I2C_I2C_DRIVER_H
#define MY_BME280_DRIVE_I2C_I2C_DRIVER_H

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdint.h>

#define I2C_ADAPTER_1    "/dev/i2c-1"  /*Address drive file in linux OS*/

class I2C_Driver{
private:
    int fd;                 /*this is file which have a "link" to linux i2c-driver file*/
    char *i2c_driver_id;    /*Address drive file in linux OS*/

public:
    I2C_Driver(char *i2c_driver_id);

    /*universal function for writing in i2c connected device
    *unsig char slave_addr - device address on i2c bus
    *unsig char reg - address of register for writing
    *unsig char data - data for writing*/
    int write_buffer_i2c(uint8_t slave_addr, uint8_t reg, uint8_t data);

    /*universal function for reading from i2c connected device
    *unsig char slave_addr - device address on i2c bus
    *unsig char reg - address of register for begining reading
    *int read_len - length of read
    *unsig char read_res[] - array for saving result of reading*/
    int read_buffer_i2c(uint8_t slave_addr, uint8_t reg, int read_len, uint8_t read_res[]);

    /*this function for getting access to driver_file*/
    int open_i2c_file();
};

#endif //MY_BME280_DRIVE_I2C_I2C_DRIVER_H

//
// Created by vovan on 23.03.2020.
//

#include "i2c_driver.h"

I2C_Driver::I2C_Driver(char *i2c_driver_id) {
    this->i2c_driver_id = i2c_driver_id;
    this->fd = -1;
}

/*this function for getting access to driver_file*/
int I2C_Driver::open_i2c_file() {
    if (fd > 0)
        return fd;
    fd = open(i2c_driver_id, O_RDWR);
    return fd;
}

/*universal function for writing in i2c connected device
*unsig char slave_addr - device addres on i2c bus
*unsig char reg - address of register for writing
*unsig char data - data for writing*/
int I2C_Driver::write_buffer_i2c(uint8_t slave_addr, uint8_t reg, uint8_t data) {
    uint8_t outbuf[2];
    struct i2c_rdwr_ioctl_data msgset[1];
    struct i2c_msg msgs[5];

    outbuf[0] = reg;
    outbuf[1] = data;

    msgs[0].addr = slave_addr;
    msgs[0].flags = 0;
    msgs[0].len = 2;
    msgs[0].buf = outbuf;

    msgset[0].msgs = msgs;
    msgset[0].nmsgs = 1;

    if (ioctl(fd, I2C_RDWR, &msgset) < 0) {
        printf("Can't WRITE in i2c_dev's register\n");
        return -1;
    }

    return 0;
}

/*universal function for reading from i2c connected device
*unsig char slave_addr - device addres on i2c bus
*unsig char reg - address of register for begining reading
*int read_len - length of read
*unsig char read_res[] - array for saving result of reading*/
int I2C_Driver::read_buffer_i2c(uint8_t slave_addr, uint8_t reg, int read_len, uint8_t read_res[]) {
    struct i2c_rdwr_ioctl_data data;
    struct i2c_msg messages[2];
    uint8_t write_buf[1] = { reg };
    //read_buf[read_len];
    for (int i = 0; i < read_len; i++) {
        read_res[i] = 0x00;
    }

    /*
     * .addr - device addres
     * .flags - opperation reading or writing (0 - w, 1 - r)
     * .len - count of messages
     * .buf - buffer for reading or writing
     */
    messages[0].addr = slave_addr;
    messages[0].flags = 0;
    messages[0].len = 1;
    messages[0].buf = write_buf;

    messages[1].addr = slave_addr;
    messages[1].flags = 1;
    messages[1].len = read_len;
    messages[1].buf = read_res;

    data.msgs = messages;
    data.nmsgs = 2;

    if (ioctl(fd, I2C_RDWR, &data) < 0) {
        printf("Can't READ data from i2c_dev's register!\n");
        return -1;
    }
    return 0;
}
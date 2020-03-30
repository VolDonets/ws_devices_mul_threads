//
// Created by vovan on 21.03.2020.
//

#ifndef MY_BME280_DRIVE_I2C_BME280CALIBRATIONDATA_H
#define MY_BME280_DRIVE_I2C_BME280CALIBRATIONDATA_H

#include <stdint.h>

class BME280CalibrationData {
private:
    uint16_t T1;
    int16_t T2;
    int16_t T3;

    uint16_t P1;
    int16_t P2;
    int16_t P3;
    int16_t P4;
    int16_t P5;
    int16_t P6;
    int16_t P7;
    int16_t P8;
    int16_t P9;

    uint8_t H1;
    int16_t H2;
    uint8_t H3;
    int16_t H4;
    int16_t H5;
    int8_t H6;

public:
    BME280CalibrationData() {
        T1 = 0;
        T2 = 0;
        T3 = 0;
        P1 = 0;
        P2 = 0;
        P3 = 0;
        P4 = 0;
        P5 = 0;
        P6 = 0;
        P7 = 0;
        P8 = 0;
        P9 = 0;
        H1 = 0;
        H2 = 0;
        H3 = 0;
        H4 = 0;
        H5 = 0;
        H6 = 0;
    }

    BME280CalibrationData(uint16_t T1, int16_t T2, int16_t T3, uint16_t P1, int16_t P2, int16_t P3, int16_t P4, int16_t P5, int16_t P6, int16_t P7, int16_t P8, int16_t P9,
                          uint8_t H1, int16_t H2, uint8_t H3, int16_t H4, int16_t H5, int8_t H6) {
        this->P1 = P1;
        this->P2 = P2;
        this->P3 = P3;
        this->P4 = P4;
        this->P5 = P5;
        this->P6 = P6;
        this->P7 = P7;
        this->P8 = P8;
        this->P9 = P9;
        this->T1 = T1;
        this->T2 = T2;
        this->T3 = T3;
        this->H1 = H1;
        this->H2 = H2;
        this->H3 = H3;
        this->H4 = H4;
        this->H5 = H5;
        this->H6 = H6;
    }

    virtual ~BME280CalibrationData() {
    }

    void setP1(uint16_t P1) {
        this->P1 = P1;
    }

    void setP2(int16_t P2) {
        this->P2 = P2;
    }

    void setP3(int16_t P3) {
        this->P3 = P3;
    }

    void setP4(int16_t P4) {
        this->P4 = P4;
    }

    void setP5(int16_t P5) {
        this->P5 = P5;
    }

    void setP6(int16_t P6) {
        this->P6 = P6;
    }

    void setP7(int16_t P7) {
        this->P7 = P7;
    }

    void setP8(int16_t P8) {
        this->P8 = P8;
    }

    void setP9(int16_t P9) {
        this->P9 = P9;
    }

    void setT1(uint16_t T1) {
        this->T1 = T1;
    }

    void setT2(int16_t T2) {
        this->T2 = T2;
    }

    void setT3(int16_t T3) {
        this->T3 = T3;
    }

    uint16_t getP1() {
        return P1;
    }

    int16_t getP2() {
        return P2;
    }

    int16_t getP3() {
        return P3;
    }

    int16_t getP4() {
        return P4;
    }

    int16_t getP5() {
        return P5;
    }

    int16_t getP6() {
        return P6;
    }

    int16_t getP7() {
        return P7;
    }

    int16_t getP8() {
        return P8;
    }

    int16_t getP9() {
        return P9;
    }

    uint16_t getT1() {
        return T1;
    }

    int16_t getT2() {
        return T2;
    }

    int16_t getT3() {
        return T3;
    }

    void setH1(uint8_t H1) {
        this->H1 = H1;
    }
    void setH2(int16_t H2) {
        this->H2 = H2;
    }
    void setH3(uint8_t H3) {
        this->H3 = H3;
    }
    void setH4(int16_t H4) {
        this->H4 = H4;
    }
    void setH5(int16_t H5) {
        this->H5 = H5;
    }
    void setH6(int8_t H6) {
        this->H6 = H6;
    }
    uint8_t getH1() {
        return H1;
    }
    int16_t getH2() {
        return H2;
    }
    uint8_t getH3() {
        return H3;
    }
    int16_t getH4() {
        return H4;
    }
    int16_t getH5() {
        return H5;
    }
    int8_t getH6() {
        return H6;
    }
};
#endif //MY_BME280_DRIVE_I2C_BME280CALIBRATIONDATA_H

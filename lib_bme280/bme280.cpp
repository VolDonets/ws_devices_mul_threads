//
// Created by vovan on 21.03.2020.
//

#include <string.h>
#include <stdint.h>
#include <stdexcept>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <zconf.h>
#include "bme280.h"

void BME280::write8(uint8_t reg, uint8_t value) {
    i2c_write(reg, value);
}

int BME280::read8(uint8_t reg) {
    uint8_t res[1];
    i2c_read(reg, 1, res);
    return res[0];
}

uint8_t BME280::readU8(uint8_t reg) {
    return (uint8_t) read8(reg);
}

int8_t BME280::readS8(uint8_t reg) {
    return (int8_t) read8(reg);
}

int BME280::read16(uint8_t reg) {
    uint8_t res[2];
    i2c_read(reg, 2, res);
    uint16_t r0 = res[0];
    uint16_t r1 = res[1];
    return (r1 << 8) | r0;
}

int16_t BME280::readS16(uint8_t reg) {
    return (int16_t) read16(reg);
}

uint16_t BME280::readU16(uint8_t reg) {
    return (uint16_t) read16(reg);
}

BME280::BME280(const char* device, int devId, i2c_init_func_def i2c_init, i2c_read_func_def i2c_read, i2c_write_func_def i2c_write):
        fd(0), chipId(0), bmp280CalibrationData(0), bmp280RawData(0) {
    this->device = new char[strlen(device)];
    strcpy(this->device, device);
    this->devId = devId;
    this->i2c_init = i2c_init;
    this->i2c_read = i2c_read;
    this->i2c_write = i2c_write;
}

BME280::BME280(int devId) :
        fd(0), chipId(0), bmp280CalibrationData(0), bmp280RawData(0) {
    this->devId = devId;
}

BME280::~BME280() {
    delete bmp280CalibrationData;
    delete bmp280RawData;
}

int BME280::init() {
    int fd = i2c_init();
    if (fd < 0) {
        char buffer[256];
        sprintf(buffer, "Device not found: device ID = %d", devId);
        throw std::logic_error(buffer);
    }
    this->fd = fd;
    uint8_t chipId = getChipId();
    switch (chipId) {
        case BME280_CHIP_ID1:
            this->chipId = chipId;
            break;
        default: {
            char buffer[256];
            sprintf(buffer, "Device Chip ID error: chip ID = %d", chipId);
            throw std::logic_error(buffer);
        }
    }
    if (bmp280CalibrationData) {
        delete bmp280CalibrationData;
    }
    bmp280CalibrationData = getCalibrationData();
    return fd;
}

BME280CalibrationData * BME280::getCalibrationData() {
    int8_t H6;
    uint8_t H1, H3;
    int16_t T2, T3, P2, P3, P4, P5, P6, P7, P8, P9, H2, H4, H5;
    uint16_t T1, P1;

    T1 = readU16(BME280_REGISTER_DIG_T1);
    T2 = readS16(BME280_REGISTER_DIG_T2);
    T3 = readS16(BME280_REGISTER_DIG_T3);
    P1 = readU16(BME280_REGISTER_DIG_P1);
    P2 = readS16(BME280_REGISTER_DIG_P2);
    P3 = readS16(BME280_REGISTER_DIG_P3);
    P4 = readS16(BME280_REGISTER_DIG_P4);
    P5 = readS16(BME280_REGISTER_DIG_P5);
    P6 = readS16(BME280_REGISTER_DIG_P6);
    P7 = readS16(BME280_REGISTER_DIG_P7);
    P8 = readS16(BME280_REGISTER_DIG_P8);
    P9 = readS16(BME280_REGISTER_DIG_P9);
    H1 = readU8(BME280_REGISTER_DIG_H1);
    H2 = readS16(BME280_REGISTER_DIG_H2);
    H3 = readU8(BME280_REGISTER_DIG_H3);
    H4 = (int16_t)((read8(BME280_REGISTER_DIG_H4) << 4) | (read8(BME280_REGISTER_DIG_H4 + 1) & 0xF));
    H5 = (int16_t)((read8(BME280_REGISTER_DIG_H5 + 1) << 4) | (read8(BME280_REGISTER_DIG_H5) >> 4));
    H6 = readS8(BME280_REGISTER_DIG_H6);

    return new BME280CalibrationData(T1, T2, T3, P1, P2, P3, P4, P5, P6, P7, P8, P9, H1, H2, H3, H4, H5, H6);
}

BME280CalibrationData * BME280::getBmp280CalibrationData() {
    return bmp280CalibrationData;
}

BME280RawData * BME280::getRawData() {
    uint8_t pmsb, plsb, pxsb;
    uint8_t tmsb, tlsb, txsb;
    uint8_t hmsb, hlsb;
    uint32_t temperature, pressure, humidity;

    plsb = readU8(BME280_REGISTER_PRESSUREDATA_LSB);
    pmsb = readU8(BME280_REGISTER_PRESSUREDATA_MSB);
    pxsb = readU8(BME280_REGISTER_PRESSUREDATA_XLSB);

    tmsb = readU8(BME280_REGISTER_TEMPDATA_MSB);
    tlsb = readU8(BME280_REGISTER_TEMPDATA_LSB);
    txsb = readU8(BME280_REGISTER_TEMPDATA_XLSB);

    hmsb = readU8(BME280_REGISTER_HUMIDDATA_MSB);
    hlsb = readU8(BME280_REGISTER_HUMIDDATA_LSB);

    temperature = 0;
    temperature = (temperature | tmsb) << 8;
    temperature = (temperature | tlsb) << 8;
    temperature = (temperature | txsb) >> 4;

    pressure = 0;
    pressure = (pressure | pmsb) << 8;
    pressure = (pressure | plsb) << 8;
    pressure = (pressure | pxsb) >> 4;

    humidity = 0;
    humidity = (humidity | hmsb) << 8;
    humidity = (humidity | hlsb);

    return new BME280RawData(pmsb, plsb, pxsb, tmsb, tlsb, txsb, hmsb, hlsb, temperature, pressure, humidity);
}

void BME280::reset() {
    setReset (BME280_SOFT_RESET_CODE);
}

void BME280::spi3wEnable() {
    uint8_t config = getConfig();
    setConfig(config | 0b00000001);
}

void BME280::spi3wDisable() {
    uint8_t config = getConfig();
    setConfig(config & 0b11111110);
}

void BME280::setPowerMode(uint8_t mode) {
    switch (mode) {
        case BME280_FORCED_MODE:
        case BME280_NORMAL_MODE:
        case BME280_SLEEP_MODE: {
            uint8_t curentMode = getControl() & 0b11111100;
            setControl(curentMode | mode);
            break;
        }
        default:
            break;
    }
}

void BME280::setTemperatureOversampling(uint8_t oversampling) {
    switch (oversampling) {
        case BME280_OVERSAMP_SKIPPED:
        case BME280_OVERSAMP_1X:
        case BME280_OVERSAMP_2X:
        case BME280_OVERSAMP_4X:
        case BME280_OVERSAMP_8X:
        case BME280_OVERSAMP_16X: {
            uint8_t curentOversampling = getControl() & 0b00011111;
            setControl(curentOversampling | (oversampling << 5));
            break;
        }
        default:
            break;
    }
}

void BME280::setPressureOversampling(uint8_t oversampling) {
    switch (oversampling) {
        case BME280_OVERSAMP_SKIPPED:
        case BME280_OVERSAMP_1X:
        case BME280_OVERSAMP_2X:
        case BME280_OVERSAMP_4X:
        case BME280_OVERSAMP_8X:
        case BME280_OVERSAMP_16X: {
            uint8_t curentOversampling = getControl() & 0b11100011;
            setControl(curentOversampling | (oversampling << 2));
            break;
        }
        default:
            break;
    }
}

void BME280::setHumidityOversampling(uint8_t oversampling) {
    switch (oversampling) {
        case BME280_OVERSAMP_SKIPPED:
        case BME280_OVERSAMP_1X:
        case BME280_OVERSAMP_2X:
        case BME280_OVERSAMP_4X:
        case BME280_OVERSAMP_8X:
        case BME280_OVERSAMP_16X: {
            setControlHumidity(0b00000111 & oversampling);
            break;
        }
        default:
            break;
    }
}

uint8_t BME280::getHumidityOversampling() {
    return getControlHumidity() & 0b00000111;
}

void BME280::setStandbyTime(uint8_t tStandby) {
    switch (tStandby) {
        case BME280_STANDBY_TIME_1_MS:
        case BME280_STANDBY_TIME_63_MS:
        case BME280_STANDBY_TIME_125_MS:
        case BME280_STANDBY_TIME_250_MS:
        case BME280_STANDBY_TIME_500_MS:
        case BME280_STANDBY_TIME_1000_MS:
        case BME280_STANDBY_TIME_2000_MS:
        case BME280_STANDBY_TIME_4000_MS: {
            uint8_t config = getConfig() & 0b00011111;
            setConfig(config | (tStandby << 5));
            break;
        }
        default:
            break;
    }
}

void BME280::setIrrFilter(uint8_t irrFilter) {
    switch (irrFilter) {
        case BME280_FILTER_COEFF_OFF:
        case BME280_FILTER_COEFF_2:
        case BME280_FILTER_COEFF_4:
        case BME280_FILTER_COEFF_8:
        case BME280_FILTER_COEFF_16: {
            uint8_t config = getConfig() & 0b11100011;
            setConfig(config | (irrFilter << 2));
            break;
        }
        default:
            break;
    }
}

uint8_t BME280::getPowerMode() {
    return getControl() & 0b00000011;
}

uint8_t BME280::getPressureOversampling() {
    return (getControl() & 0b00011100) >> 2;
}

uint8_t BME280::getTemperatureOversampling() {
    return (getControl() & 0b11100000) >> 5;
}

uint8_t BME280::getIrrFilter() {
    return (getConfig() & 0b00011100) >> 2;
}

uint8_t BME280::getStandbyTime() {
    return (getConfig() & 0b11100000) >> 5;
}

uint8_t BME280::getSpi3w() {
    return (getConfig() & 0b00000001) >> 5;
}

uint8_t BME280::getMeasuringStatus() {
    return (getStatus() >> 3) & 0b00000001;
}

uint8_t BME280::getImUpdateStatus() {
    return getStatus() & 0b00000001;
}

uint8_t BME280::getConfig() {
    return readU8(BME280_REGISTER_CONFIG);
}

uint8_t BME280::getStatus() {
    return readU8(BME280_REGISTER_STATUS);
}

uint8_t BME280::getControl() {
    return readU8(BME280_REGISTER_CONTROL);
}

uint8_t BME280::getControlHumidity() {
    return readU8(BME280_REGISTER_CONTROLHUMID);
}

uint8_t BME280::getChipId() {
    return readU8(BME280_REGISTER_CHIPID);
}

uint8_t BME280::getChipVersion() {
    return readU8(BME280_REGISTER_VERSION);
}

void BME280::setReset(uint8_t value) {
    write8(BME280_REGISTER_SOFTRESET, value);
}

void BME280::setConfig(uint8_t value) {
    write8(BME280_REGISTER_CONFIG, value);
}

void BME280::setStatus(uint8_t value) {
    write8(BME280_REGISTER_STATUS, value);
}

void BME280::setControl(uint8_t value) {
    write8(BME280_REGISTER_CONTROL, value);
}

void BME280::setControlHumidity(uint8_t value) {
    write8(BME280_REGISTER_CONTROLHUMID, value);
}

double BME280::getAltitude(double pressure) {
    return 44330.0 * (1.0 - pow(pressure / MEAN_SEA_LEVEL_PRESSURE, 0.190294957));
}

int32_t BME280::getTemperatureC(int32_t adc_T) {
    int32_t var1 = ((((adc_T >> 3) - ((int32_t) bmp280CalibrationData->getT1() << 1))) * ((int32_t) bmp280CalibrationData->getT2())) >> 11;

    int32_t var2 = (((((adc_T >> 4) - ((int32_t) bmp280CalibrationData->getT1())) * ((adc_T >> 4) - ((int32_t) bmp280CalibrationData->getT1()))) >> 12)
                    * ((int32_t) bmp280CalibrationData->getT3())) >> 14;

    return var1 + var2;
}

double BME280::compensateTemperature(int32_t t_fine) {
    double T = (t_fine * 5 + 128) >> 8;
    return T / 100;
}

double BME280::compensatePressure(int32_t adc_P, int32_t t_fine) {
    int64_t var1, var2, p;

    var1 = ((int64_t) t_fine) - 128000;
    var2 = var1 * var1 * (int64_t) bmp280CalibrationData->getP6();
    var2 = var2 + ((var1 * (int64_t) bmp280CalibrationData->getP5()) << 17);
    var2 = var2 + (((int64_t) bmp280CalibrationData->getP4()) << 35);
    var1 = ((var1 * var1 * (int64_t) bmp280CalibrationData->getP3()) >> 8) + ((var1 * (int64_t) bmp280CalibrationData->getP2()) << 12);
    var1 = (((((int64_t) 1) << 47) + var1)) * ((int64_t) bmp280CalibrationData->getP1()) >> 33;

    if (var1 == 0) {
        return 0;  // avoid exception caused by division by zero
    }
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t) bmp280CalibrationData->getP9()) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t) bmp280CalibrationData->getP8()) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t) bmp280CalibrationData->getP7()) << 4);
    return (double) p / 256;
}

double BME280::compensateHumidity(int32_t adc_H, int32_t t_fine) {
    int32_t v_x1_u32r;

    v_x1_u32r = (t_fine - ((int32_t) 76800));

    v_x1_u32r = (((((adc_H << 14) - (((int32_t) bmp280CalibrationData->getH4()) << 20) - (((int32_t) bmp280CalibrationData->getH5()) * v_x1_u32r)) + ((int32_t) 16384)) >> 15)
                 * (((((((v_x1_u32r * ((int32_t) bmp280CalibrationData->getH6())) >> 10) * (((v_x1_u32r * ((int32_t) bmp280CalibrationData->getH3())) >> 11) + ((int32_t) 32768))) >> 10)
                      + ((int32_t) 2097152)) * ((int32_t) bmp280CalibrationData->getH2()) + 8192) >> 14));

    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t) bmp280CalibrationData->getH1())) >> 4));

    v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
    v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
    double h = (v_x1_u32r >> 12);
    return h / 1024.0;
}

BMP280Data * BME280::getBMP280Data() {
    int32_t t_fine;
    double t, p, h, a;
    while (getMeasuringStatus()) {
    }
    if (bmp280RawData) {
        delete bmp280RawData;
    }
    bmp280RawData = getRawData();
    t_fine = getTemperatureC(bmp280RawData->getTemperature());
    t = compensateTemperature(t_fine); // C
    p = compensatePressure(bmp280RawData->getPressure(), t_fine) / 100; // hPa
    h = compensateHumidity(bmp280RawData->getHumidity(), t_fine);
    a = getAltitude(p);                         // meters

    return new BMP280Data(p, t, h, a);
}


void BME280::defaultInit() {
    init();
    reset();
    reset();
    setPowerMode(BME280_NORMAL_MODE);
    setTemperatureOversampling(BME280_ULTRAHIGHRESOLUTION_OVERSAMP_TEMPERATURE);
    setPressureOversampling(BME280_ULTRAHIGHRESOLUTION_OVERSAMP_PRESSURE);
    setHumidityOversampling(BME280_ULTRAHIGHRESOLUTION_OVERSAMP_HUMIDITY);
    setIrrFilter(BME280_FILTER_COEFF_16);
    setStandbyTime(BME280_STANDBY_TIME_250_MS);
}

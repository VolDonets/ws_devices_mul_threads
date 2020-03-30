//
// Created by vovan on 21.03.2020.
//

#ifndef MY_BME280_DRIVE_I2C_BME280_H
#define MY_BME280_DRIVE_I2C_BME280_H

#include <stdint.h>
#include "BME280CalibrationData.h"
#include "BME280RawData.h"
#include "BME280Data.h"

#define MEAN_SEA_LEVEL_PRESSURE       1013

/**\name    CHIP ID DEFINITION       */
/***********************************************/
#define BME280_CHIP_ID1     (0x60)
/************************************************/
/**\name    I2C ADDRESS DEFINITION       */
/***********************************************/
#define BME280_I2C_ADDRESS1                  (0x76)
#define BME280_I2C_ADDRESS2                  (0x77)
/************************************************/
/**\name    POWER MODE DEFINITION       */
/***********************************************/
/* Sensor Specific constants */
#define BME280_SLEEP_MODE                    (0x00)
#define BME280_FORCED_MODE                   (0x01)
#define BME280_NORMAL_MODE                   (0x03)
#define BME280_SOFT_RESET_CODE               (0xB6)
/************************************************/
/**\name    STANDBY TIME DEFINITION       */
/***********************************************/
#define BME280_STANDBY_TIME_1_MS              (0x00)
#define BME280_STANDBY_TIME_63_MS             (0x01)
#define BME280_STANDBY_TIME_125_MS            (0x02)
#define BME280_STANDBY_TIME_250_MS            (0x03)
#define BME280_STANDBY_TIME_500_MS            (0x04)
#define BME280_STANDBY_TIME_1000_MS           (0x05)
#define BME280_STANDBY_TIME_2000_MS           (0x06)
#define BME280_STANDBY_TIME_4000_MS           (0x07)
/************************************************/
/**\name    OVERSAMPLING DEFINITION       */
/***********************************************/
#define BME280_OVERSAMP_SKIPPED          (0x00)
#define BME280_OVERSAMP_1X               (0x01)
#define BME280_OVERSAMP_2X               (0x02)
#define BME280_OVERSAMP_4X               (0x03)
#define BME280_OVERSAMP_8X               (0x04)
#define BME280_OVERSAMP_16X              (0x05)
/************************************************/
/**\name    WORKING MODE DEFINITION       */
/***********************************************/
#define BME280_ULTRA_LOW_POWER_MODE          (0x00)
#define BME280_LOW_POWER_MODE                (0x01)
#define BME280_STANDARD_RESOLUTION_MODE      (0x02)
#define BME280_HIGH_RESOLUTION_MODE          (0x03)
#define BME280_ULTRA_HIGH_RESOLUTION_MODE    (0x04)

#define BME280_ULTRALOWPOWER_OVERSAMP_PRESSURE          BME280_OVERSAMP_1X
#define BME280_ULTRALOWPOWER_OVERSAMP_HUMIDITY          BME280_OVERSAMP_1X
#define BME280_ULTRALOWPOWER_OVERSAMP_TEMPERATURE       BME280_OVERSAMP_1X

#define BME280_LOWPOWER_OVERSAMP_PRESSURE               BME280_OVERSAMP_2X
#define BME280_LOWPOWER_OVERSAMP_HUMIDITY               BME280_OVERSAMP_2X
#define BME280_LOWPOWER_OVERSAMP_TEMPERATURE            BME280_OVERSAMP_1X

#define BME280_STANDARDRESOLUTION_OVERSAMP_PRESSURE     BME280_OVERSAMP_4X
#define BME280_STANDARDRESOLUTION_OVERSAMP_HUMIDITY     BME280_OVERSAMP_4X
#define BME280_STANDARDRESOLUTION_OVERSAMP_TEMPERATURE  BME280_OVERSAMP_1X

#define BME280_HIGHRESOLUTION_OVERSAMP_PRESSURE         BME280_OVERSAMP_8X
#define BME280_HIGHRESOLUTION_OVERSAMP_HUMIDITY         BME280_OVERSAMP_8X
#define BME280_HIGHRESOLUTION_OVERSAMP_TEMPERATURE      BME280_OVERSAMP_1X

#define BME280_ULTRAHIGHRESOLUTION_OVERSAMP_PRESSURE    BME280_OVERSAMP_16X
#define BME280_ULTRAHIGHRESOLUTION_OVERSAMP_HUMIDITY    BME280_OVERSAMP_16X
#define BME280_ULTRAHIGHRESOLUTION_OVERSAMP_TEMPERATURE BME280_OVERSAMP_2X
/************************************************/
/**\name    FILTER DEFINITION       */
/***********************************************/
#define BME280_FILTER_COEFF_OFF               (0x00)
#define BME280_FILTER_COEFF_2                 (0x01)
#define BME280_FILTER_COEFF_4                 (0x02)
#define BME280_FILTER_COEFF_8                 (0x03)
#define BME280_FILTER_COEFF_16                (0x04)
/************************************************/

/*
 * REGISTERS
 */
#define BME280_ADDRESS                      0x76
#define BME280_REGISTER_DIG_T1              0x88
#define BME280_REGISTER_DIG_T2              0x8A
#define BME280_REGISTER_DIG_T3              0x8C
#define BME280_REGISTER_DIG_P1              0x8E
#define BME280_REGISTER_DIG_P2              0x90
#define BME280_REGISTER_DIG_P3              0x92
#define BME280_REGISTER_DIG_P4              0x94
#define BME280_REGISTER_DIG_P5              0x96
#define BME280_REGISTER_DIG_P6              0x98
#define BME280_REGISTER_DIG_P7              0x9A
#define BME280_REGISTER_DIG_P8              0x9C
#define BME280_REGISTER_DIG_P9              0x9E
#define BME280_REGISTER_DIG_H1              0xA1
#define BME280_REGISTER_DIG_H2              0xE1
#define BME280_REGISTER_DIG_H3              0xE3
#define BME280_REGISTER_DIG_H4              0xE4
#define BME280_REGISTER_DIG_H5              0xE5
#define BME280_REGISTER_DIG_H6              0xE7
#define BME280_REGISTER_CHIPID              0xD0
#define BME280_REGISTER_VERSION             0xD1
#define BME280_REGISTER_SOFTRESET           0xE0
#define BME280_RESET                        0xB6
#define BME280_REGISTER_CAL26               0xE1
#define BME280_REGISTER_CONTROLHUMID        0xF2
#define BME280_REGISTER_STATUS              0xF3
#define BME280_REGISTER_CONTROL             0xF4
#define BME280_REGISTER_CONFIG              0xF5
#define BME280_REGISTER_PRESSUREDATA_MSB    0xF7
#define BME280_REGISTER_PRESSUREDATA_LSB    0xF8
#define BME280_REGISTER_PRESSUREDATA_XLSB   0xF9
#define BME280_REGISTER_TEMPDATA_MSB        0xFA
#define BME280_REGISTER_TEMPDATA_LSB        0xFB
#define BME280_REGISTER_TEMPDATA_XLSB       0xFC
#define BME280_REGISTER_HUMIDDATA_MSB       0xFD
#define BME280_REGISTER_HUMIDDATA_LSB       0xFE

typedef int (*i2c_write_func_def)(uint8_t reg, uint8_t data);
typedef int (*i2c_read_func_def)(uint8_t reg, int read_len, uint8_t read_res[]);
typedef int (*i2c_init_func_def)();

class BME280 {
private:
    char * device;
    int devId;
    int fd;
    uint8_t chipId;
    BME280CalibrationData * bmp280CalibrationData;
    BME280RawData * bmp280RawData;
    i2c_init_func_def i2c_init;
    i2c_read_func_def i2c_read;
    i2c_write_func_def i2c_write;

    void write8(uint8_t, uint8_t);
    int read8(uint8_t);
    uint8_t readU8(uint8_t);
    int8_t readS8(uint8_t);
    int read16(uint8_t);
    int16_t readS16(uint8_t);
    uint16_t readU16(uint8_t);

    int32_t getTemperatureC(int32_t adc_T);
    double getAltitude(double pressure);
    double compensateTemperature(int32_t t_fine);
    double compensatePressure(int32_t adc_P, int32_t t_fine);
    double compensateHumidity(int32_t adc_H, int32_t t_fine);
    BME280CalibrationData * getCalibrationData();
    BME280RawData * getRawData();

public:
    BME280(const char*, int, i2c_init_func_def, i2c_read_func_def, i2c_write_func_def);
    BME280(int);
    virtual ~BME280();

    BME280CalibrationData * getBmp280CalibrationData();
    BMP280Data * getBMP280Data();

    int init();
    void reset();
    void spi3wEnable();
    void spi3wDisable();
    void setPowerMode(uint8_t);
    void setTemperatureOversampling(uint8_t);
    void setPressureOversampling(uint8_t);
    void setHumidityOversampling(uint8_t);
    void setStandbyTime(uint8_t);
    void setIrrFilter(uint8_t);
    uint8_t getPowerMode();
    uint8_t getPressureOversampling();
    uint8_t getHumidityOversampling();
    uint8_t getTemperatureOversampling();
    uint8_t getIrrFilter();
    uint8_t getStandbyTime();
    uint8_t getSpi3w();
    uint8_t getMeasuringStatus();
    uint8_t getImUpdateStatus();
    uint8_t getConfig();
    uint8_t getStatus();
    uint8_t getControl();
    uint8_t getControlHumidity();
    uint8_t getChipId();
    uint8_t getChipVersion();
    void setReset(uint8_t);
    void setConfig(uint8_t);
    void setStatus(uint8_t);
    void setControl(uint8_t);
    void setControlHumidity(uint8_t);

    void defaultInit();
};

#endif //MY_BME280_DRIVE_I2C_BME280_H

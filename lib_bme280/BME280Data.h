//
// Created by vovan on 21.03.2020.
//

#ifndef MY_BME280_DRIVE_I2C_BME280DATA_H
#define MY_BME280_DRIVE_I2C_BME280DATA_H

class BMP280Data {
private:
    double pressure;    // hPa
    double temperature; // m
    double humidity;    // %
    double altitude;    // °C
public:
    BMP280Data() {
        pressure = 0;
        temperature = 0;
        altitude = 0;
        humidity = 0;
    }
    BMP280Data(double pressure, double temperature, double humidity, double altitude) {
        this->pressure = pressure;
        this->temperature = temperature;
        this->humidity = humidity;
        this->altitude = altitude;
    }
    virtual ~BMP280Data() {
    }
    void setAltitude(double altitude) {
        this->altitude = altitude;
    }
    void setPressure(double pressure) {
        this->pressure = pressure;
    }
    void setTemperature(double temperature) {
        this->temperature = temperature;
    }
    double getAltitude() {
        return altitude;
    }
    double getPressure() {
        return pressure;
    }
    double getTemperature() {
        return temperature;
    }
    void setHumidity(double humidity) {
        this->humidity = humidity;
    }
    double getHumidity() {
        return humidity;
    }
};

#endif //MY_BME280_DRIVE_I2C_BME280DATA_H

#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <ICM20948_WE.h>

class IMUManager {
public:
    IMUManager(TwoWire &w = Wire, uint8_t addr = 0x68);
    void begin();
    void update();

    // Gesamtwerte
    float getTemperature() const;
    float getResultantG() const;

    // Achsenweise Zugriff
    float getAccX() const;
    float getAccY() const;
    float getAccZ() const;

    float getGyroX() const;
    float getGyroY() const;
    float getGyroZ() const;

    float getMagX() const;
    float getMagY() const;
    float getMagZ() const;

private:
    TwoWire* _wire;
    uint8_t _addr;
    ICM20948_WE* _imu;

    xyzFloat acc;
    xyzFloat gyr;
    xyzFloat mag;
    float temp;
    float resultantG;

    void configureSensor();
};

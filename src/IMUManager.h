#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <ICM20948_WE.h>

class IMUManager {
public:
    IMUManager(TwoWire &w = Wire, uint8_t addr = 0x68);
    //~IMUManager();
    void begin();
    void update();

    // Gesamtwerte
    float getTemperature() const;
    float getResultantG() const;

    // ACC
    float getAccX() const;
    float getAccY() const;
    float getAccZ() const;

    // GYRO
    float getGyroX() const;
    float getGyroY() const;
    float getGyroZ() const;

    // MAG
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

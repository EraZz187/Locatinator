#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <ICM20948_WE.h>
#include <EEPROM.h>

class IMUManager {
public:
    IMUManager(TwoWire &wire, uint8_t addr = 0x68);
    ~IMUManager();

    void begin();
    void update();
    void calibrateMagnetometer(bool saveToEEPROM = true);
    float getHeading() const;

    // Getter
    float getTemperature() const;
    float getResultantG() const;
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
    void configureSensor();
    void loadMagOffsetsFromEEPROM();
    void saveMagOffsetsToEEPROM();

    TwoWire *_wire;
    uint8_t _addr;
    ICM20948_WE *_imu;

    xyzFloat acc{}, gyr{}, mag{};
    xyzFloat magOffset{};
    float temp = 0;
    float resultantG = 0;
};

#pragma once
#define DEBUG_

#include <ICM20948_WE.h>

class IMUManager {
public:
    IMUManager(int csPin);
    void begin();
    float getTemperature();
    float getAccX();
    float getAccY();
    float getAccZ();
    float getGyroX();
    float getGyroY();
    float getGyroZ();
    float getMagX();
    float getMagY();
    float getMagZ();

private:
    ICM20948_WE imu;
    int csPin;
};

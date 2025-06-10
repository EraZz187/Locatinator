#pragma once

#include <ICM20948_WE.h>
#include <SPI.h>

class IMUManager {
public:
    IMUManager(SPIClass* spi, int csPin, int mosiPin, int misoPin, int sckPin, bool useSpi);

    void begin();
    float getTemperature();
    float getAccX(), getAccY(), getAccZ();
    float getGyroX(), getGyroY(), getGyroZ();
    float getMagX(), getMagY(), getMagZ();

private:
    SPIClass* spi;
    ICM20948_WE imu;

    int csPin, mosiPin, misoPin, sckPin;
    bool useSpi;
};

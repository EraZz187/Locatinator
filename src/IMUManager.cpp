#include "IMUManager.h"

IMUManager::IMUManager(TwoWire &w, uint8_t addr) {
    _wire = &w;
    _addr = addr;
    _imu = new ICM20948_WE(_wire, _addr);
}

void IMUManager::begin() {
    _wire->begin();
    delay(200);

    if (!_imu->init()) {
        Serial.println("ICM20948 not responding.");
        return;
    }

    if (!_imu->initMagnetometer()) {
        Serial.println("Magnetometer not responding.");
    }

    configureSensor();
    update();
}

void IMUManager::configureSensor() {
    _imu->setAccRange(ICM20948_ACC_RANGE_2G);
    _imu->setAccDLPF(ICM20948_DLPF_6);
    _imu->setGyrDLPF(ICM20948_DLPF_6);
    _imu->setTempDLPF(ICM20948_DLPF_6);
    _imu->setMagOpMode(AK09916_CONT_MODE_20HZ);
}

void IMUManager::update() {
    _imu->readSensor();
    _imu->getGValues(&acc);
    _imu->getGyrValues(&gyr);
    _imu->getMagValues(&mag);
    temp = _imu->getTemperature();
    resultantG = _imu->getResultantG(&acc);
}

// Temperaturen & Summen
float IMUManager::getTemperature() const { return temp; }
float IMUManager::getResultantG() const { return resultantG; }

// ACC
float IMUManager::getAccX() const { return acc.x; }
float IMUManager::getAccY() const { return acc.y; }
float IMUManager::getAccZ() const { return acc.z; }

// GYRO
float IMUManager::getGyroX() const { return gyr.x; }
float IMUManager::getGyroY() const { return gyr.y; }
float IMUManager::getGyroZ() const { return gyr.z; }

// MAG
float IMUManager::getMagX() const { return mag.x; }
float IMUManager::getMagY() const { return mag.y; }
float IMUManager::getMagZ() const { return mag.z; }

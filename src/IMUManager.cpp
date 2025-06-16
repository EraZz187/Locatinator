#include "IMUManager.h"

IMUManager::IMUManager(TwoWire &wire, uint8_t addr)
    : _wire(&wire), _addr(addr), _imu(new ICM20948_WE(_wire, _addr)) {};

//IMUManager::~IMUManager() {
//    delete _imu;
//}

void IMUManager::begin() {
    _wire->begin();
    delay(100);

    if (!_imu->init()) {
        Serial.println("❌ ICM20948 not responding.");
        return;
    }

    if (!_imu->initMagnetometer()) {
        Serial.println("⚠️ Magnetometer not responding.");
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

float IMUManager::getTemperature() const { return temp; }
float IMUManager::getResultantG() const { return resultantG; }
float IMUManager::getAccX() const { return acc.x; }
float IMUManager::getAccY() const { return acc.y; }
float IMUManager::getAccZ() const { return acc.z; }
float IMUManager::getGyroX() const { return gyr.x; }
float IMUManager::getGyroY() const { return gyr.y; }
float IMUManager::getGyroZ() const { return gyr.z; }
float IMUManager::getMagX() const { return mag.x; }
float IMUManager::getMagY() const { return mag.y; }
float IMUManager::getMagZ() const { return mag.z; }
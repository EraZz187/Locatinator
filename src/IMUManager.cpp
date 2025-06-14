#include "IMUManager.h"

IMUManager::IMUManager(TwoWire &wire, uint8_t addr)
    : _wire(&wire), _addr(addr), _imu(new ICM20948_WE(_wire, _addr)) {};

IMUManager::~IMUManager() {
    delete _imu;
}

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

#if defined(ESP32) || defined(ESP8266)
    EEPROM.begin(sizeof(xyzFloat));
#endif

    loadMagOffsetsFromEEPROM();
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

float IMUManager::getHeading() const {
    float correctedX = mag.x - magOffset.x;
    float correctedY = mag.y - magOffset.y;
    float heading = atan2(correctedY, correctedX) * 180.0 / PI;
    return (heading < 0) ? heading + 360 : heading;
}

void IMUManager::calibrateMagnetometer(bool saveToEEPROM) {

    float minX = 9999, maxX = -9999;
    float minY = 9999, maxY = -9999;
    float minZ = 9999, maxZ = -9999;

    unsigned long start = millis();
    while (millis() - start < 10000) {
        update();
        minX = min(minX, mag.x); maxX = max(maxX, mag.x);
        minY = min(minY, mag.y); maxY = max(maxY, mag.y);
        minZ = min(minZ, mag.z); maxZ = max(maxZ, mag.z);
        delay(50);
    }

    magOffset.x = (maxX + minX) / 2.0;
    magOffset.y = (maxY + minY) / 2.0;
    magOffset.z = (maxZ + minZ) / 2.0;
    if (saveToEEPROM) saveMagOffsetsToEEPROM();
}

void IMUManager::saveMagOffsetsToEEPROM() {
    EEPROM.put(0, magOffset);
#if defined(ESP32) || defined(ESP8266)
    EEPROM.commit();
#endif
}

void IMUManager::loadMagOffsetsFromEEPROM() {
    EEPROM.get(0, magOffset);
    if (abs(magOffset.x) > 1000 || abs(magOffset.y) > 1000) {
        Serial.println("⚠️ EEPROM enthält keine gültigen Offsets. Bitte kalibrieren!");
        magOffset = {0, 0, 0};
    }
}

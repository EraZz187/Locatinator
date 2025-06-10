#include "IMUManager.h"
#include <SPI.h>

IMUManager::IMUManager(int CS_PIN, int MOSI_PIN, int MISO_PIN, int SCK_PIN, bool USE_SPI)
    : imu(&SPI, CS_PIN, MOSI_PIN, MISO_PIN, SCK_PIN, USE_SPI), CS_PIN(CS_PIN), MOSI_PIN(MOSI_PIN), MISO_PIN(MISO_PIN), SCK_PIN(SCK_PIN),  USE_SPI(USE_SPI){}

void IMUManager::begin()
{
  if (!imu.init())
  {
    Serial.println("ICM20948 not responding");
  }
  if (!imu.initMagnetometer())
  {
    Serial.println("Magnetometer not responding");
  }

  imu.setAccDLPF(ICM20948_DLPF_6);
  imu.setGyrSampleRateDivider(10);
  imu.setTempDLPF(ICM20948_DLPF_6);
  imu.setMagOpMode(AK09916_CONT_MODE_20HZ);
}

float IMUManager::getTemperature()
{
  imu.readSensor();
  return imu.getTemperature();
}

float IMUManager::getAccX()
{
  imu.readSensor();
  xyzFloat acc;
  imu.getGValues(&acc);
  return acc.x;
}

float IMUManager::getAccY()
{
  imu.readSensor();
  xyzFloat acc;
  imu.getGValues(&acc);
  return acc.y;
}

float IMUManager::getAccZ()
{
  imu.readSensor();
  xyzFloat acc;
  imu.getGValues(&acc);
  return acc.z;
}

float IMUManager::getGyroX()
{
  imu.readSensor();
  xyzFloat gyr;
  imu.getGyrValues(&gyr);
  return gyr.x;
}

float IMUManager::getGyroY()
{
  imu.readSensor();
  xyzFloat gyr;
  imu.getGyrValues(&gyr);
  return gyr.y;
}

float IMUManager::getGyroZ()
{
  imu.readSensor();
  xyzFloat gyr;
  imu.getGyrValues(&gyr);
  return gyr.z;
}

float IMUManager::getMagX()
{
  imu.readSensor();
  xyzFloat mag;
  imu.getMagValues(&mag);
  return mag.x;
}

float IMUManager::getMagY()
{
  imu.readSensor();
  xyzFloat mag;
  imu.getMagValues(&mag);
  return mag.y;
}

float IMUManager::getMagZ()
{
  imu.readSensor();
  xyzFloat mag;
  imu.getMagValues(&mag);
  return mag.z;
}

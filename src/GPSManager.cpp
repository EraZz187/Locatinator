#include "GPSManager.h"
#include <Arduino.h>

GPSManager::GPSManager(uint8_t uartNumber)
    : uartNumber(uartNumber)
{
    gpsSerial = new HardwareSerial(uartNumber);
}

void GPSManager::begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin) {
    gpsSerial->begin(baud, config, rxPin, txPin);
    Serial.println("📡 UART gestartet!");
}

void GPSManager::update() {
    while (gpsSerial->available() > 0) {
        gps.encode(gpsSerial->read());
    }
}

uint8_t GPSManager::getSatelitesCount() {
    return gps.satellites.value();
}

float GPSManager::getLatitude() {
    return gps.location.isValid() ? gps.location.lat() : 0.0f;
}

float GPSManager::getLongitude(){
    return gps.location.isValid() ? gps.location.lng() : 0.0f;
}

float GPSManager::getAltitude() {
    return gps.altitude.isValid() ? gps.altitude.meters() : 0.0f;
}

GPSManager::dateTime GPSManager::getDateTime() {
    dateTime dt;
    memset(&dt, 0, sizeof(dt));

    if (gps.date.isValid() && gps.time.isValid()) {
        dt.year        = gps.date.year();
        dt.month       = gps.date.month();
        dt.day         = gps.date.day();
        dt.hour        = gps.time.hour();
        dt.minute      = gps.time.minute();
        dt.second      = gps.time.second();
        dt.centisecond = gps.time.centisecond();
    }

    return dt;
}

bool GPSManager::hasFix() {
    return gps.location.isValid() && gps.time.isValid();
}

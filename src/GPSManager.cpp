#include "GPSManager.h"
#include <Arduino.h>

GPSManager::GPSManager() 
    : gps() {}

void GPSManager::begin() {
    Serial.println("📡 GPS gestartet!");
}

void GPSManager::update(Stream &gpsStream) {
    while (gpsStream.available() > 0) {
        gps.encode(gpsStream.read());
    }
}

uint8_t GPSManager::getSatelitesCount() {
    return gps.satellites.value();
}

float GPSManager::getLatitude() {
    return gps.location.isValid() ? gps.location.lat() : 0.0f;
}

float GPSManager::getLongitude() {
    return gps.location.isValid() ? gps.location.lng() : 0.0f;
}

float GPSManager::getAltitude() {
    return gps.altitude.isValid() ? gps.altitude.meters() : 0.0f;
}

GPSManager::dateTime GPSManager::getDateTime() {
    dateTime dt = {0};

    if (gps.date.isValid() && gps.time.isValid()) {
        dt.day = gps.date.day();
        dt.month = gps.date.month();
        dt.year = gps.date.year();
        dt.hour = gps.time.hour();
        dt.minute = gps.time.minute();
        dt.second = gps.time.second();
        dt.centisecond = gps.time.centisecond();
    }

    return dt;
}

bool GPSManager::hasFix() {
    return gps.location.isValid() && gps.time.isValid();
}
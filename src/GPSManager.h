#pragma once
#define DEBUG_

#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

class GPSManager {
public:
    struct dateTime {
        uint16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        uint8_t centisecond;
    };

    GPSManager(uint8_t uartNumber); // z. B. 1 oder 2
    void begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin);
    void update();

    uint8_t getSatelitesCount();
    float getLatitude() ;
    float getLongitude();
    float getAltitude();
    dateTime getDateTime();
    bool hasFix();
    
    HardwareSerial* gpsSerial;

private:
    uint8_t uartNumber;
    TinyGPSPlus gps;
};
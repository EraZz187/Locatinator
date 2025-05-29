#pragma once
#define DEBUG_

#include <TinyGPSPlus.h>

class GPSManager {
public:
    struct dateTime
    {
        uint8_t day;
        uint8_t month;
        uint16_t year;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        uint16_t centisecond;
    };

    GPSManager(u_int8_t uartPin);
    void begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin);
    void update(Stream &gpsStream);     // <- WICHTIG: GPS-Daten einspeisen

    uint8_t getSatelitesCount();
    float getLatitude();
    float getLongitude();
    float getAltitude();
    dateTime getDateTime();

    bool hasFix();                      // Optional: liefert true, wenn gültiger GPS-Fix vorliegt

private:
    u_int8_t uartPin;
    HardwareSerial GPSSerial;
    TinyGPSPlus gps;
};

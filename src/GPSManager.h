#pragma once
#include <TinyGPSPlus.h>

class GPSManager
{
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

    GPSManager();
    void begin();
    void update(Stream &gpsStream);     // <- WICHTIG: GPS-Daten einspeisen

    uint8_t getSatelitesCount();
    float getLatitude();
    float getLongitude();
    float getAltitude();
    dateTime getDateTime();

    bool hasFix();                      // Optional: liefert true, wenn gültiger GPS-Fix vorliegt

private:
    TinyGPSPlus gps;
};

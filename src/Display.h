#pragma once

#include <TFT_eSPI.h>
#include "GPSManager.h"
#include "IMUManager.h"

class Display
{
public:
    Display();
    void init();
    void update(GPSManager &gps, IMUManager &imu);
    void showMessage(const char *msg);
    void resetMessages();
    void setDeclination(float degrees);

private:
    TFT_eSPI tft;
    int currentMessageLine = 0;
    int maxLines = 0;
    int lineHeight = 20;
    float declination = 0.0;
    float lastLat = 0.0f;
    float lastLon = 0.0f;

    int drawSensorData(GPSManager &gps, IMUManager &imu);
    void printLine(const char *msg, int line);
    void updateMaxLines();
    void drawCompass(const IMUManager &imu, int baseLine);
    float estimateDeclination(float lat, float lon);
};

#pragma once

#include <TFT_eSPI.h>
#include "GPSManager.h"
#include "IMUManager.h"

class Display {
public:
public:
    Display();
    void init();
    void update(GPSManager &gps,  IMUManager &imu);
    void showMessage(const char* msg);

private:
    TFT_eSPI tft;
    void drawLayout();
    void drawSensorData( GPSManager &gps, IMUManager &imu);
    void printLine(const char* msg, int line);
};

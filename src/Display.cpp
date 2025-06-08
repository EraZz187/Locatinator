#include "Display.h"


Display::Display()
    : tft()  // TFT_eSPI verwendet Setup aus User_Setup.h
{}

void Display::init() {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    drawLayout();
}

void Display::update( GPSManager &gps, IMUManager &imu) {
    drawSensorData(gps, imu);
}

void Display::drawLayout() {
    tft.fillScreen(TFT_BLACK);
    printLine("GPS:", 0);
    printLine("IMU:", 4);
}

void Display::drawSensorData( GPSManager &gps, IMUManager &imu) {
    float lat = gps.getLatitude();
    float lon = gps.getLongitude();
    float temp = imu.getTemperature();

    printLine(("Lat: " + String(lat, 6)).c_str(), 1);
    printLine(("Lon: " + String(lon, 6)).c_str(), 2);
    printLine(("Temp: " + String(temp, 1) + " C").c_str(), 5);

}

void Display::printLine(const char* msg, int line) {
    int y = 20 + line * 20;
    tft.fillRect(0, y, 320, 20, TFT_BLACK);
    tft.setCursor(10, y);
    tft.println(msg);
}

void Display::showMessage(const char* msg) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 40);
    tft.println(msg);
}
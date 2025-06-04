#include "Display.h"

Display::Display()
    : tft()  // TFT_eSPI verwendet Setup aus User_Setup.h
{}

void Display::init() {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
}

void Display::showMessage(const char* msg) {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(20, 40);
    tft.println(msg);
}
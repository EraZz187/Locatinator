#include "Display.h"

Display::Display(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t mosi, uint8_t sck)
    : tft(cs, dc, rst), _mosi(mosi), _sck(sck)
{}

void Display::init() {
    SPI.begin(_sck, -1, _mosi);  // SCK, MISO (-1 = unused), MOSI

    tft.begin();  // ILI9488 via SPI
    tft.setRotation(1);
    tft.fillScreen(ILI9488_BLACK);
}

void Display::showMessage(const char* msg) {
    tft.setTextColor(ILI9488_WHITE, ILI9488_BLACK);
    tft.setTextSize(2);
    tft.setCursor(20, 40);
    tft.println(msg);
}
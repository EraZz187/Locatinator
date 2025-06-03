#pragma once

#include <ILI9488.h>

class Display {
public:
    Display(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t mosi, uint8_t sck);
    void init();
    void showMessage(const char* msg);

private:
    uint8_t _mosi, _sck;
    ILI9488 tft;
};

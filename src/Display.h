#pragma once

#include <TFT_eSPI.h>

class Display {
public:
    Display();
    void init();
    void showMessage(const char* msg);

private:
    TFT_eSPI tft;
};

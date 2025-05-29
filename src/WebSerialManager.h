#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

namespace WebSerialManager {
    void begin(AsyncWebServer* server);
}
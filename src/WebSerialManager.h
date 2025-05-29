#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

namespace WebSerialManager {
    void begin(AsyncWebServer* server);
    void write(const String& message);
    void println(const String& message);
}

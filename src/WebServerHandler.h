#pragma once

#include <ESPAsyncWebServer.h>
#include "GPSManager.h"
#include "IMUManager.h"

void setupWebServer(AsyncWebServer &server, GPSManager &gps, IMUManager &imu, long &tick);
String loadHtmlFile(const char *path);

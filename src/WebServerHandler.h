#pragma once
#include <WiFi.h>
#include <GPSManager.h>
#include <IMUManager.h>

void setupWebServer(WiFiServer &server, GPSManager &gps, IMUManager &imu);
void handleClient(WiFiServer &server, const String &htmlPage);
void routeRequest(const String &request, WiFiClient &client);
String loadHtmlFile(const char *path);
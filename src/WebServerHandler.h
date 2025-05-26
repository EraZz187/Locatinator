#pragma once
#include <WiFi.h>
#include <GPSManager.h>
#include <IMUManager.h>

void setupWebServer(WiFiServer &server, GPSManager &gps, IMUManager &imu);
void handleClient(WiFiServer &server, const String &htmlPage, GPSManager &gps, IMUManager &imu, long tick);
void routeRequest(const String &request, WiFiClient &client, GPSManager &gps, IMUManager &imu, long tick);
String loadHtmlFile(const char *path);
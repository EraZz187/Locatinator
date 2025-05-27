#include "WebServerHandler.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>

// Hilfsfunktion zum Bereinigen von float-Werten (NAN oder INF -> 0.0)
float sanitize(float val) {
  if (isnan(val) || isinf(val)) return 0.0;
  return val;
}

void setupWebServer(AsyncWebServer &server, GPSManager &gps, IMUManager &imu, long &tick)
{
  // Sensor-Daten als JSON ausgeben
  server.on("/sensor", HTTP_GET, [&](AsyncWebServerRequest *request){
    JsonDocument doc = StaticJsonDocument<512>();

    doc["SAT_COUNT"] = gps.getSatelitesCount();
    doc["LAT"] = sanitize(gps.getLatitude());
    doc["LON"] = sanitize(gps.getLongitude());
    doc["ALT"] = sanitize(gps.getAltitude());
    doc["TEMP"] = sanitize(imu.getTemperature());
    doc["ACCX"] = sanitize(imu.getAccX());
    doc["ACCY"] = sanitize(imu.getAccY());
    doc["ACCZ"] = sanitize(imu.getAccZ());
    doc["GYROX"] = sanitize(imu.getGyroX());
    doc["GYROY"] = sanitize(imu.getGyroY());
    doc["GYROZ"] = sanitize(imu.getGyroZ());
    doc["MAGX"] = sanitize(imu.getMagX());
    doc["MAGY"] = sanitize(imu.getMagY());
    doc["MAGZ"] = sanitize(imu.getMagZ());
    doc["TICK"] = tick;

    String json;
    serializeJson(doc, json);
    request->send(200, "application/json", json);
    Serial.println("✅ JSON-Daten gesendet, tick=" + String(tick));
  });

  // Statische Dateien automatisch aus SPIFFS ausliefern
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  // Optional: 404-Handler
  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/html", "<h1>Datei nicht gefunden</h1>");
  });

  server.begin();
}

String loadHtmlFile(const char *path)
{
  File file = SPIFFS.open(path, "r");
  if (!file || file.isDirectory())
    return "<h1>Datei nicht gefunden</h1>";

  String content = file.readString();
  file.close();

  content.replace("{{STA_IP}}", WiFi.localIP().toString());
  content.replace("{{AP_IP}}", WiFi.softAPIP().toString());

  return content;
}
#include <Arduino.h>
#include "WiFiManager.h"
#include "OTAHandler.h"
#include "mDNSHandler.h"
#include "WebServerHandler.h"
#include <SPIFFS.h>
#include "GPSManager.h"
#include "IMUManager.h"
#include "WebServerHandler.h"

#define DEBUG_

// Zugangsdaten und Netzwerkkonfiguration
const char *ap_ssid = "locatinator";
const char *ap_password = "12345678";
const char *sta_ssid = "TurkishAirlines_";
const char *sta_password = "123Polizei";
const char *sta_hostname = "locatinator";

IPAddress local_IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

AsyncWebServer server(80);
GPSManager gps;
IMUManager imu(5);

String cachedHtml;

long tick = 0;

void setup()
{
  Serial.begin(115200);
#ifdef DEBUG_
  Serial.println("Booting");
#endif

  if (!SPIFFS.begin(true))
  { // true = format if mount fails
#ifdef DEBUG_
    Serial.println("❌ SPIFFS konnte nicht gemountet werden!");
#endif
  }
  else
  {
#ifdef DEBUG_    
  Serial.println("✅ SPIFFS gemountet!");
#endif
  }

#ifdef DEBUG_
  if (SPIFFS.exists("/index.html"))
  {
    Serial.println("✅ index.html gefunden!");
  }
  else
  {
    Serial.println("❌ index.html fehlt im SPIFFS!");
  }
#endif

  imu.begin();

  //(connectToWiFi(sta_ssid, sta_password, 10); // Verbindet sich mit dem WLAN (Station-Modus)
  startAccessPoint(ap_ssid, ap_password, local_IP, gateway, subnet); // Startet eigenen Access Point (AP-Modus)
  setupWebServer(server, gps, imu, tick);
  setupMDNS(sta_hostname); // Startet mDNS-Dienst (z. B. locatinator.local erreichbar)
  setupOTA(sta_hostname);  // Initialisiert OTA (Over-the-Air) Updates

  cachedHtml = loadHtmlFile("/index.html"); // HTML-Seite aus SPIFFS laden und im RAM cachen
}

void loop()
{
  tick++;
  ArduinoOTA.handle();                                     // gibt dem internen Task-Scheduler des ESP32 Zeit, andere Prozesse laufen zu lassen -- verhindert Abstürze durch blockierende Schleifen
}
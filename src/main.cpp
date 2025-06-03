#include <Arduino.h>
#include "WiFiManager.h"
#include "OTAHandler.h"
#include "mDNSHandler.h"
#include "WebServerHandler.h"
#include <SPIFFS.h>
#include "GPSManager.h"
#include "IMUManager.h"
#include "WebServerHandler.h"
#include "WebSerialManager.h"
#include "Display.h"

// Debug
#define DEBUG_
long tick = 0;

// SPI-Pinbelegung
#define TFT_MOSI 23
#define TFT_SCK  18

// GPS
GPSManager gps(2);
#define RXD_GPS 16
#define TXD_GPS 17
#define GPS_BAUD 9600

//TFT Monitor
#define TFT_CS   15
#define TFT_DC   2
#define TFT_RST  4

Display display(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCK);

// IMU
IMUManager imu(5);

// Network
const char *ap_ssid = "locatinator_AP";
const char *ap_password = "12345678";
const char *sta_ssid = "TurkishAirlines_";
const char *sta_password = "123Polizei";
const char *sta_hostname = "locatinator";

IPAddress local_IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);


AsyncWebServer server(80);

String cachedHtml;

void setup()
{
  Serial.begin(115200);
#ifdef DEBUG_
  Serial.println("Booting");
#endif

  if (!SPIFFS.begin(true))
  {
    Serial.println("❌ SPIFFS konnte nicht gemountet werden!");
  }
  else
  {
    Serial.println("✅ SPIFFS gemountet!");
  }

  if (SPIFFS.exists("/index.html"))
  {
    Serial.println("✅ index.html gefunden!");
  }
  else
  {
    Serial.println("❌ index.html fehlt im SPIFFS!");
  }

  imu.begin();
  gps.begin(GPS_BAUD, SERIAL_8N1, RXD_GPS, TXD_GPS);

  //TFT Monitor
    display.init();
    display.showMessage("Hallo ILI9488!");


  WebSerialManager::begin(&server);

  //connectToWiFi(sta_ssid, sta_password, 10);                         // Verbindet sich mit dem WLAN (Station-Modus)
  startAccessPoint(ap_ssid, ap_password, local_IP, gateway, subnet); // Startet eigenen Access Point (AP-Modus)
  setupWebServer(server, gps, imu, tick);
  setupMDNS(sta_hostname); // Startet mDNS-Dienst (z. B. locatinator.local erreichbar)
  setupOTA(sta_hostname);  // Initialisiert OTA (Over-the-Air) Updates

  cachedHtml = loadHtmlFile("/index.html"); // HTML-Seite aus SPIFFS laden und im RAM cachen
}

void loop()
{
  tick++;
  ArduinoOTA.handle(); // gibt dem internen Task-Scheduler des ESP32 Zeit, andere Prozesse laufen zu lassen -- verhindert Abstürze durch blockierende Schleifen
  gps.update();
  WebSerialManager::println(String(gps.getSatelitesCount()));
  delay(1000);
}
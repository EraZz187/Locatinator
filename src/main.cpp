#include <Arduino.h>
#include "WiFiManager.h"
#include "OTAHandler.h"
#include "mDNSHandler.h"
#include "WebServerHandler.h"
#include <SPIFFS.h>
#include "GPSManager.h"
#include "IMUManager.h"

// Zugangsdaten und Netzwerkkonfiguration
const char *ap_ssid = "locatinator";
const char *ap_password = "12345678";
const char *sta_ssid = "TurkishAirlines_";
const char *sta_password = "123Polizei";
const char *sta_hostname = "locatinator";

IPAddress local_IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiServer server(80);
GPSManager gps;
IMUManager imu(5);

String cachedHtml;

void setup()
{
  Serial.begin(115200);
  Serial.println("Booting");
  if(!SPIFFS.begin(true)) {  // true = format if mount fails
    Serial.println("❌ SPIFFS konnte nicht gemountet werden!");
  } else {
    Serial.println("✅ SPIFFS gemountet!");
  }

  if (SPIFFS.exists("/index.html")) {
  Serial.println("✅ index.html gefunden!");
  } else {
    Serial.println("❌ index.html fehlt im SPIFFS!");
  }

  imu.begin();

  connectToWiFi(sta_ssid, sta_password, 10);                         // Verbindet sich mit dem WLAN (Station-Modus)
  startAccessPoint(ap_ssid, ap_password, local_IP, gateway, subnet); // Startet eigenen Access Point (AP-Modus)
  setupWebServer(server, gps, imu);
  setupOTA(sta_hostname);                                            // Initialisiert OTA (Over-the-Air) Updates
  setupMDNS(sta_hostname);                                           // Startet mDNS-Dienst (z. B. locatinator.local erreichbar)

  cachedHtml = loadHtmlFile("/index.html");                          // HTML-Seite aus SPIFFS laden und im RAM cachen
}

void loop()
{
  handleOTA();                                                       // Handhabt eingehende OTA-Anfragen
  handleClient(server, cachedHtml);                                  // Webserver: Clientanfragen verarbeiten und HTML-Seite ausliefern
}

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
char temp[255];
String sWebSerial;

// TFT Monitor
Display display;

// GPS
GPSManager gps(2);
#define RXD_GPS 16
#define TXD_GPS 17
#define GPS_BAUD 9600

// IMU
IMUManager imu(Wire); // ICM20948 address is 0x68 by default

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
  delay(1000);
  display.showMessage("Booting...");

  if (!SPIFFS.begin(true))
  {
     display.showMessage(" SPIFFS konnte nicht gemountet werden!");
  }
  else
  {
     display.showMessage(" SPIFFS gemountet!");
  }

  if (SPIFFS.exists("/index.html"))
  {
     display.showMessage(" index.html gefunden!");
  }
  else
  {
     display.showMessage(" index.html fehlt im SPIFFS!");
  }

  display.init();

  display.showMessage("Init IMU...");
  imu.begin();

  display.showMessage("Init GPS...");
  gps.begin(GPS_BAUD, SERIAL_8N1, RXD_GPS, TXD_GPS);
  delay(1000);

  display.showMessage("Init WebSerial...");
  WebSerialManager::begin(&server);

  // connectToWiFi(sta_ssid, sta_password, 10); // Verbindet sich mit dem WLAN (Station-Modus)
  display.showMessage("Init Access Point...");
  startAccessPoint(ap_ssid, ap_password, local_IP, gateway, subnet); // Startet eigenen Access Point (AP-Modus)

  display.showMessage("Init Webserver...");
  setupWebServer(server, gps, imu, tick);

  display.showMessage("Init mDNS...");
  setupMDNS(sta_hostname);
  
  char buffer[64];
  String message = "Webserver erreichbar unter: http://" + String(sta_hostname) + ".local";
  message.toCharArray(buffer, sizeof(buffer));
  display.showMessage(buffer);

  message = "IP-Adresse: " + local_IP.toString();
  message.toCharArray(buffer, sizeof(buffer));
  display.showMessage(buffer);

  display.showMessage("Init OTA...");
  setupOTA(sta_hostname);

  display.showMessage("HTML-Seite aus SPIFFS laden und im RAM cachen ...");
  cachedHtml = loadHtmlFile("/index.html");
  delay(5000);
  display.resetMessages();
}

void loop()
{
  tick++;
  ArduinoOTA.handle(); // gibt dem internen Task-Scheduler des ESP32 Zeit, andere Prozesse laufen zu lassen -- verhindert Abstürze durch blockierende Schleifen
  gps.update();
  imu.update();
  display.update(gps, imu);

  
  WebSerialManager::println("-----GPS-----");
  WebSerialManager::println("Rohe GPS-Daten: " + gps.rawData());
  Serial.println("Rohe GPS-Daten: " + gps.rawData());
  WebSerialManager::println("Gefundene Sateliten: " + String(gps.getSatelitesCount()));

  sprintf(temp, "%02d.%02d.%02d", gps.getDateTime().day, gps.getDateTime().month, gps.getDateTime().year);
  sWebSerial = String(temp);
  WebSerialManager::println("Datum: " + sWebSerial);

  sprintf(temp, "%02d.%02d.%02d", gps.getDateTime().hour, gps.getDateTime().minute, gps.getDateTime().second);
  sWebSerial = String(temp);
  WebSerialManager::println("Zeit (UTC): " + sWebSerial);

  WebSerialManager::println("Höhe: " + String(gps.getAltitude()) + "m.ü.M");
  WebSerialManager::println("Längengrad: " + String(gps.getLongitude()));
  WebSerialManager::println("Breitengrad: " + String(gps.getLatitude()));

  WebSerialManager::println("-----IMU-----");
  WebSerialManager::println("Temperatur: " + String(imu.getTemperature()));

  WebSerialManager::println("Beschleunigungssensor X: " + String(imu.getAccX()));
  WebSerialManager::println("Beschleunigungssensor Y: " + String(imu.getAccY()));
  WebSerialManager::println("Beschleunigungssensor Z: " + String(imu.getAccZ()));
  WebSerialManager::println("Resultierende G Kraft: " + String(imu.getResultantG()));

  WebSerialManager::println("Gyroskop X: " + String(imu.getGyroX()));
  WebSerialManager::println("Gyroskop Y: " + String(imu.getGyroY()));
  WebSerialManager::println("Gyroskop Z: " + String(imu.getGyroZ()));

  WebSerialManager::println("Magnetometer X: " + String(imu.getMagX()));
  WebSerialManager::println("Magnetometer Y: " + String(imu.getMagY()));
  WebSerialManager::println("Magnetometer Z: " + String(imu.getMagZ()));

  delay(500);
}
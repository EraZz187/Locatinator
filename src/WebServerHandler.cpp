#include "WebServerHandler.h"
#include <WiFi.h>
#include "WiFiManager.h"
#include "SPIFFS.h"
#include "GPSManager.h"
#include "IMUManager.h"

// --- Initialisierung des Webservers inkl. Sensorreferenzen ---
void setupWebServer(WiFiServer &server, GPSManager &gps, IMUManager &imu)
{
  server.begin();
}

// --- Hauptfunktion zur Clientverarbeitung ---
void handleClient(WiFiServer &server, const String &htmlPage, GPSManager &gps, IMUManager &imu, long tick)
{
  WiFiClient client = server.available();
  if (!client)
    return;
  
  String request = "";
  while (client.connected())
  {
    if (client.available())
    {
      char c = client.read();
      request += c;

      //Serial.println("Request erhalten:\n" + request);

      if (request.endsWith("\r\n\r\n"))
      {                                // Anfrage
        //routeRequest(request, client, gps, imu, tick); // Aufruf Router
        break;
      }
    }
  }

  client.flush();
  client.stop();
  Serial.println("❎ Client getrennt");
}

// --- Routing: Liefert JSON oder statische Dateien aus ---
void routeRequest(const String &request, WiFiClient &client, GPSManager &gps, IMUManager &imu, long tick)
{
  String path = "/index.html"; // Standard

  int firstLineEnd = request.indexOf("\r\n");
  String firstLine = request.substring(0, firstLineEnd);

  int firstSpace = firstLine.indexOf(' ');
  int secondSpace = firstLine.indexOf(' ', firstSpace + 1);
  if (firstSpace >= 0 && secondSpace > firstSpace)
  {
    path = firstLine.substring(firstSpace + 1, secondSpace);
    if (path == "/")
      path = "/index.html";
  }

  // === Sensor-Daten als JSON ausgeben ===
  if (path == "/sensor")
  {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();

    client.println("{");
    client.print("\"SAT_COUNT\": ");
    client.print(gps.getSatelitesCount());
    client.println(",");
    client.print("\"LAT\": ");
    client.print(gps.getLatitude(), 6);
    client.println(",");
    client.print("\"LON\": ");
    client.print(gps.getLongitude(), 6);
    client.println(",");
    client.print("\"ALT\": ");
    client.print(gps.getAltitude(), 1);
    client.println(",");
    client.print("\"TEMP\": ");
    client.print(imu.getTemperature(), 1);
    client.println(",");
    client.print("\"ACCX\": ");
    client.print(imu.getAccX(), 2);
    client.println(",");
    client.print("\"ACCY\": ");
    client.print(imu.getAccY(), 2);
    client.println(",");
    client.print("\"ACCZ\": ");
    client.print(imu.getAccZ(), 2);
    client.println(",");
    client.print("\"GYROX\": ");
    client.print(imu.getGyroX(), 2);
    client.println(",");
    client.print("\"GYROY\": ");
    client.print(imu.getGyroY(), 2);
    client.println(",");
    client.print("\"GYROZ\": ");
    client.print(imu.getGyroZ(), 2);
    client.println(",");
    client.print("\"MAGX\": ");
    client.print(imu.getMagX(), 2);
    client.println(",");
    client.print("\"MAGY\": ");
    client.print(imu.getMagY(), 2);
    client.println(",");
    client.print("\"MAGZ\": ");
    client.print(imu.getMagZ(), 2);
    client.println(",");
    client.print("\"TICK\": ");
    client.print(tick, 2);
    client.println("\n}");


    Serial.println("\n" + tick);
    return;
  }

  // Versuche statische Datei zu laden
  Serial.println("📥 Angeforderter Pfad: " + path);
  File file = SPIFFS.open(path);
  if (!file || file.isDirectory())
  {
    client.println("HTTP/1.1 404 Not Found");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<h1>Datei nicht gefunden</h1>");
    return;
  }

  String contentType = "text/html";
  if (path.endsWith(".js"))
    contentType = "application/javascript";
  else if (path.endsWith(".css"))
    contentType = "text/css";
  else if (path.endsWith(".json"))
    contentType = "application/json";
  else if (path.endsWith(".png"))
    contentType = "image/png";
  else if (path.endsWith(".jpg") || path.endsWith(".jpeg"))
    contentType = "image/jpeg";

  client.println("HTTP/1.1 200 OK");
  client.print("Content-Type: ");
  client.println(contentType);
  client.println("Connection: close");
  client.println();

  while (file.available())
  {
    client.write(file.read());
  }
  file.close();
}

String loadHtmlFile(const char *path)
{
  File file = SPIFFS.open(path);
  if (!file || file.isDirectory())
    return "<h1>Datei nicht gefunden</h1>";
  String content = file.readString();
  file.close();
  content.replace("{{STA_IP}}", WiFi.localIP().toString());
  content.replace("{{AP_IP}}", WiFi.softAPIP().toString());
  return content;
}
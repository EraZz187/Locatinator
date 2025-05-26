#include "WebServerHandler.h"
#include <SPIFFS.h>

// Starte den Webserver
void setupWebServer(WiFiServer &server, GPSManager &gps, IMUManager &imu)
{
  server.begin();
}

// Bearbeite ankommende Client-Verbindung
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

      if (request.endsWith("\r\n\r\n"))
      {
        routeRequest(request, client, gps, imu, tick);
        break;
      }
    }
  }

  client.flush();
  client.stop();
#ifdef DEBUG_
  Serial.println("❎ Client getrennt");
#endif
}

// Hilfsfunktion zum Bereinigen von float-Werten (NAN oder INF -> 0.0)
float sanitize(float val) {
  if (isnan(val) || isinf(val)) return 0.0;
  return val;
}

// Routenhandler: JSON oder statische Dateien liefern
void routeRequest(const String &request, WiFiClient &client, GPSManager &gps, IMUManager &imu, long tick)
{
  String path = "/index.html";

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

  if (path == "/sensor")
  {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();

    client.println("{");
    client.printf("\"SAT_COUNT\": %d,\n", gps.getSatelitesCount());
    client.printf("\"LAT\": %.6f,\n", sanitize(gps.getLatitude()));
    client.printf("\"LON\": %.6f,\n", sanitize(gps.getLongitude()));
    client.printf("\"ALT\": %.1f,\n", sanitize(gps.getAltitude()));
    client.printf("\"TEMP\": %.1f,\n", sanitize(imu.getTemperature()));
    client.printf("\"ACCX\": %.2f,\n", sanitize(imu.getAccX()));
    client.printf("\"ACCY\": %.2f,\n", sanitize(imu.getAccY()));
    client.printf("\"ACCZ\": %.2f,\n", sanitize(imu.getAccZ()));
    client.printf("\"GYROX\": %.2f,\n", sanitize(imu.getGyroX()));
    client.printf("\"GYROY\": %.2f,\n", sanitize(imu.getGyroY()));
    client.printf("\"GYROZ\": %.2f,\n", sanitize(imu.getGyroZ()));
    client.printf("\"MAGX\": %.2f,\n", sanitize(imu.getMagX()));
    client.printf("\"MAGY\": %.2f,\n", sanitize(imu.getMagY()));
    client.printf("\"MAGZ\": %.2f,\n", sanitize(imu.getMagZ()));
    client.printf("\"TICK\": %ld\n", tick);
    client.println("}");

    Serial.println("JSON-Daten gesendet, tick=" + String(tick));
    return;
  }

  // Statische Datei aus SPIFFS ausliefern
  Serial.println("📥 Angeforderter Pfad: " + path);
  File file = SPIFFS.open(path, "r");
  if (!file || file.isDirectory())
  {
    client.println("HTTP/1.1 404 Not Found");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<h1>Datei nicht gefunden</h1>");
    return;
  }

  // Content-Type ermitteln
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

  // Datei chunkweise senden (um WDT zu vermeiden)
  const size_t CHUNK_SIZE = 128;
  uint8_t buffer[CHUNK_SIZE];
  while (file.available())
  {
    size_t toSend = (file.available() < CHUNK_SIZE) ? file.available() : CHUNK_SIZE;
    file.read(buffer, toSend);

    if (!client.connected())
      break;

    client.write(buffer, toSend);

    yield(); // Watchdog-Timeout verhindern
  }

  file.close();
  client.stop();
}

// HTML-Datei mit Platzhaltern laden (IP-Adressen ersetzen)
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
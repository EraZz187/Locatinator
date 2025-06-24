#include "Display.h"
#include <math.h>

Display::Display()
    : tft() {}

void Display::init()
{
    tft.init();
    tft.setRotation(0); // Hochformat
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    updateMaxLines();
}

void Display::update(GPSManager &gps, IMUManager &imu)
{
    float lat = gps.getLatitude();
    float lon = gps.getLongitude();

    if (gps.getSatelitesCount() == 0 || lat == 0.0f || lon == 0.0f)
    {
        lat = 47.3769f;
        lon = 8.5417f;
    }

    declination = estimateDeclination(lat, lon);

    int lastLine = drawSensorData(gps, imu);
    drawCompass(imu, lastLine);
}

int Display::drawSensorData(GPSManager &gps, IMUManager &imu)
{
    int y = 0;
    char temp[9];
    String d, t;

    // GPS
    if (gps.getSatelitesCount() > 0)
    {
        sprintf(temp, "%02d.%02d.%02d", gps.getDateTime().day, gps.getDateTime().month, gps.getDateTime().year);
        d = String(temp);
        sprintf(temp, "%02d:%02d:%02d", gps.getDateTime().hour, gps.getDateTime().minute, gps.getDateTime().second);
        t = String(temp);
    }
    else
    {
        sprintf(temp, "%02d.%02d.%02d", 0, 0, 0);
        d = String(temp);
        sprintf(temp, "%02d:%02d:%02d", 0, 0, 0);
        t = String(temp);
    }

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    printLine("GPS-Sensordaten:", y++);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    printLine(("Gefundene Satelliten: " + String(gps.getSatelitesCount())).c_str(), y++);
    printLine(("Datum: " + String(d)).c_str(), y++);
    printLine(("Zeit (UTC): " + String(t)).c_str(), y++);
    printLine(("Hoehe: " + String(gps.getAltitude(), 1) + " m.u.M").c_str(), y++);
    printLine(("Breitengrad: " + String(gps.getLatitude(), 6)).c_str(), y++);
    printLine(("Laengengrad: " + String(gps.getLongitude(), 6)).c_str(), y++);
    y++;

    // IMU
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    printLine("IMU-Sensordaten:", y++);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    printLine(("Temperatur: " + String(imu.getTemperature(), 1) + " C").c_str(), y++);
    printLine(("Magnetsensor X: " + String(imu.getMagX(), 3)).c_str(), y++);
    printLine(("Magnetsensor Y: " + String(imu.getMagY(), 3)).c_str(), y++);
    printLine(("Magnetsensor Z: " + String(imu.getMagZ(), 3)).c_str(), y++);

    return y;
}

void Display::printLine(const char *msg, int line)
{
    int y = 20 + line * 20;
    tft.fillRect(0, y, 320, 20, TFT_BLACK);
    tft.setCursor(10, y);
    tft.println(msg);
}

void Display::showMessage(const char *msg)
{
    String text(msg);
    int maxPixelWidth = tft.width() - 20;

    while (text.length() > 0)
    {
        int len = text.length();
        int fit = len;

        while (fit > 0 && tft.textWidth(text.substring(0, fit)) > maxPixelWidth)
        {
            fit--;
        }

        int breakAt = text.substring(0, fit).lastIndexOf(' ');
        if (breakAt > 0 && fit < len)
        {
            fit = breakAt;
        }

        String line = text.substring(0, fit);
        text = text.substring(fit);
        text.trim();

        if (currentMessageLine >= maxLines)
        {
            resetMessages();
        }

        int y = currentMessageLine * lineHeight;
        tft.setCursor(10, y);
        tft.println(line);
        currentMessageLine++;
    }
}

void Display::resetMessages()
{
    tft.fillScreen(TFT_BLACK);
    currentMessageLine = 0;
}

void Display::updateMaxLines()
{
    int height = tft.height();
    maxLines = height / lineHeight;
}

float Display::estimateDeclination(float lat, float lon)
{
    if (lat > 40 && lat < 55 && lon > 5 && lon < 15)
    {
        return 2.5; // Deutschland
    }
    else if (lat > 35 && lat < 45 && lon > -10 && lon < 0)
    {
        return 0.5; // Spanien/Portugal
    }
    else if (lat > 30 && lat < 50 && lon < -90)
    {
        return -6.0; // USA Midwest
    }
    else if (lat > 25 && lat < 50 && lon > -80 && lon < -70)
    {
        return -13.0; // USA East
    }
    else if (lat > -45 && lat < -10 && lon > 110 && lon < 155)
    {
        return 11.0; // Australien
    }
    return 0.0;
}

void Display::drawCompass(const IMUManager &imu, int baseLine)
{
    const int lineHeight = 20;
    const int radius = 50;
    const int fontHeight = 8;
    const int padding = 10;

    int textEndY = 20 + (baseLine + 1) * lineHeight;
    int centerX = tft.width() / 2;
    int centerY = textEndY + padding + radius;

    // Prüfen, ob es aus dem Display läuft
    int totalBottom = centerY + radius + fontHeight + 10;
    if (totalBottom > tft.height())
    {
        centerY = tft.height() - radius - fontHeight - 10;
    }

    // Nur Kompassbereich leeren
    tft.fillRect(0, textEndY, tft.width(), tft.height() - textEndY, TFT_BLACK);

    // Kreis zeichnen
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawCircle(centerX, centerY, radius, TFT_WHITE);

    // Himmelsrichtungen
    tft.drawCentreString("N", centerX + 1, centerY - radius - fontHeight - 8, 1);
    tft.drawCentreString("E", centerX + radius + 9, centerY - 7, 1);
    tft.drawCentreString("S", centerX + 2, centerY + radius + 3, 1);
    tft.drawCentreString("W", centerX - radius - 6, centerY - 6, 1);

    // Heading berechnen
    float x = imu.getMagX();
    float y = imu.getMagY();
    float heading = atan2(y, x) * 180.0 / PI;
    if (heading < 0)
        heading += 360;
    heading += declination;
    if (heading >= 360)
        heading -= 360;

    float angle = heading * DEG_TO_RAD;
    int arrowX = centerX + radius * sin(angle);
    int arrowY = centerY - radius * cos(angle);

    tft.drawLine(centerX, centerY, arrowX, arrowY, TFT_RED);
    tft.drawLine(centerX + 1, centerY, arrowX + 1, arrowY, TFT_RED);

    // Gradtext
    char headingText[30];
    snprintf(headingText, sizeof(headingText), "%.0f Nord", heading);
    tft.drawCentreString(headingText, centerX, centerY + radius + fontHeight + 16, 1);
}
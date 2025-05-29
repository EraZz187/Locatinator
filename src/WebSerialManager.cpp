#include "WebSerialManager.h"
#include <WebSerial.h>

namespace WebSerialManager
{

    // Optional: Verarbeitung empfangener Daten
    void onWebSerialReceive(uint8_t *data, size_t len)
    {
        String received;
        for (size_t i = 0; i < len; i++)
        {
            received += (char)data[i];
        }
        Serial.println("[WebSerial] Empfang: " + received);
        WebSerial.println("Echo: " + received);
    }

    void begin(AsyncWebServer *server)
    {
        WebSerial.begin(server);
        WebSerial.onMessage(onWebSerialReceive); // ✅ korrekt
        WebSerial.println("WebSerial gestartet.");
    }
}
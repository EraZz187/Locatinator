# 📍 Locatinator

**Locatinator** ist ein ESP32-basiertes System zur Standortbestimmung, Bewegungsanalyse und Darstellung auf einem Display sowie über ein Webinterface. Es kombiniert GPS-Daten, IMU-Sensorik und Netzwerkkommunikation in einem kompakten Projekt.

---

## 🚀 Funktionen

- **GPS-Ortung** mit TinyGPSPlus  
- **Bewegungserkennung** über ICM20948 IMU  
- **Datenanzeige** auf einem TFT-Display  
- **Webserver** mit asynchronem Zugriff (ESPAsyncWebServer)  
- **Serielle Kommunikation** über WebSerial  
- **SPIFFS**-Dateisystem für lokale Daten

---

## 🔧 Verwendete Hardware

- 🧠 ESP32 DevKit v1  
- 📡 GPS-Modul (z. B. NEO-6M)  
- 🧭 ICM20948 9-Achsen IMU  
- 📺 TFT-Display (kompatibel mit TFT_eSPI)  
- 📶 WLAN (integriert im ESP32)

---

## 🛠️ Setup & Installation

### Voraussetzungen

- [VS Code](https://code.visualstudio.com/)
- [PlatformIO IDE](https://platformio.org/install/ide?install=vscode)

### Installation

```bash
# Projekt in PlatformIO öffnen und Board verbinden
pio run --target upload
pio device monitor -b 115200
```

---

## ⚙️ Konfiguration (`platformio.ini`)

```ini
[env:esp32doit-devkit-v1]
platform = espressif32
board = esp32doit-devkit-v1
framework = arduino
monitor_speed = 115200
board_build.filesystem = spiffs
build_flags = -DCORE_DEBUG_LEVEL=3
  -Wno-deprecated-declarations
monitor_filters = esp32_exception_decoder

lib_deps = 
  mikalhart/TinyGPSPlus @ 1.1.0
  wollewald/ICM20948_WE @ 1.2.5
  bblanchon/ArduinoJson @ 7.4.1
  esp32async/ESPAsyncWebServer @ 3.7.7 
  ayushsharma82/WebSerial @ 2.1.1
  bodmer/TFT_eSPI @ 2.5.43
```

---

## 🧪 Projektstruktur

```
Locatinator/
├── src/
│   └── main.cpp              # Hauptlogik
├── lib/                      # Bibliotheken (falls vorhanden)
├── include/                  # Header-Dateien
├── data/                     # SPIFFS-Daten
├── platformio.ini            # Projektkonfiguration
```

---

## 🌐 Webinterface

- Webserver erreichbar über:
  ```
  http://locatinator.local/
  ```

- Serielle Ausgabe im Web via WebSerial:
  ```
  http://locatinator.local/webserial
  ```

- OTA-Updates vorbereitbar mit:
  ```ini
  ;upload_protocol = espota
  ;upload_port = locatinator.local
  ```

---

## 📦 Libraries im Einsatz

| Library | Beschreibung |
|--------|--------------|
| [TinyGPSPlus](https://github.com/mikalhart/TinyGPSPlus) | GPS-Datenverarbeitung |
| [ICM20948_WE](https://github.com/wollewald/ICM20948_WE) | 9-Achsen-IMU |
| [ArduinoJson](https://arduinojson.org/) | JSON-Verarbeitung |
| [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) | Asynchroner Webserver |
| [WebSerial](https://github.com/ayushsharma82/WebSerial) | Serielle Ausgabe im Web |
| [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) | Display-Steuerung |

---

## 🧩 Erweiterungsideen

- Integration von MQTT für IoT-Anbindung  
- Logging auf SPIFFS oder SD-Karte  
- Kartenanzeige mit Koordinaten  
- Mobile App zur Fernabfrage

---

## 👤 Autor

**Becir Bahtiyari** 
**Patric Moser**  
**Christoph Leu** 
**Dario Meier**  

---

## ❓ Hilfe & Support

Bei Fragen oder Problemen:
- GitHub Issues erstellen
- Oder direkt über [GitHub-Profil](https://github.com/EraZz187) kontaktieren

#include "PairingLogger.h"
#include <BLEDevice.h>
#include <Adafruit_SSD1306.h>
#include <SD.h>

extern Adafruit_SSD1306 display;

// Pairing Logger Configuration
String logFileName = "/pairing_logs.txt";

void PairingLogger::start() {
  Serial.println("Starting Pairing Logger...");

  BLEServer *server = BLEDevice::createServer();
  server->setCallbacks(new class : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
      Serial.println("Device paired.");

      File logFile = SD.open(logFileName.c_str(), FILE_APPEND);
      if (logFile) {
        logFile.println("Device paired.");
        logFile.close();
      } else {
        Serial.println("Failed to log pairing.");
      }

      // Update OLED
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Pairing Logged");
      display.display();
    }
  });

  server->getAdvertising()->start();
  Serial.println("Pairing Logger active.");
}
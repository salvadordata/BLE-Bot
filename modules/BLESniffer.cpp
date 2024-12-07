#include "BLESniffer.h"
#include <BLEDevice.h>
#include <Adafruit_SSD1306.h>
#include <SD.h>

extern Adafruit_SSD1306 display;

// BLE Sniffer Configuration
int scanDuration = 5;  // Default scan duration in seconds

void BLESniffer::start() {
  Serial.println("Starting BLE Sniffer...");

  BLEScan *scanner = BLEDevice::getScan();
  scanner->setActiveScan(true);
  BLEScanResults results = scanner->start(scanDuration, false);

  // Display results on OLED and log to SD card
  File logFile = SD.open("/sniffer_logs.txt", FILE_APPEND);
  if (!logFile) {
    Serial.println("Failed to open log file.");
    return;
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Sniffer Results:");
  display.display();

  for (int i = 0; i < results.getCount(); i++) {
    BLEAdvertisedDevice device = results.getDevice(i);
    String logEntry = "Device: " + device.getAddress().toString() +
                      ", RSSI: " + String(device.getRSSI());

    if (device.haveManufacturerData()) {
      logEntry += ", Mfg Data: " + String(device.getManufacturerData().c_str());
    }

    Serial.println(logEntry);
    logFile.println(logEntry);

    // Display on OLED (limit to screen size)
    if (i < 5) {  // Show only first 5 entries on screen
      display.setCursor(0, 10 * (i + 1));
      display.println(logEntry);
      display.display();
    }
  }

  logFile.close();
  Serial.println("Sniffer logs saved.");
}
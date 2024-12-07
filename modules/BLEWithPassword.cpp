#include "BLEWithPassword.h"
#include <BLEDevice.h>
#include <Adafruit_SSD1306.h>
#include <SD.h>

extern Adafruit_SSD1306 display;

// BLE with Password Configuration
String advertisingUUID = "12345678-1234-1234-1234-123456789abc";
String defaultPassword = "Password123";

void BLEWithPassword::start() {
  Serial.println("Starting BLE with Password...");

  BLEAdvertising *advertiser = BLEDevice::getAdvertising();
  advertiser->addServiceUUID(advertisingUUID);
  advertiser->addManufacturerData(defaultPassword);
  advertiser->start();

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("BLE w/ Password Active");
  display.setCursor(0, 10);
  display.println("UUID: " + advertisingUUID);
  display.setCursor(0, 20);
  display.println("Password: " + defaultPassword);
  display.display();

  Serial.println("BLE with Password active.");
}

void BLEWithPassword::logPassword(const String &password) {
  File file = SD.open("/passwords.txt", FILE_APPEND);
  if (file) {
    file.println(password);
    file.close();
    Serial.println("Password logged.");
  } else {
    Serial.println("Failed to log password.");
  }
}
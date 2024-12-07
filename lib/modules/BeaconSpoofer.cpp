#include "BeaconSpoofer.h"
#include <BLEDevice.h>
#include <Adafruit_SSD1306.h>
#include <SD.h>

extern Adafruit_SSD1306 display;

// Spoofing Configuration
String spoofedUUID = "12345678-1234-1234-1234-123456789abc";
int advertisingInterval = 100;  // in milliseconds
int numberOfDevices = 5;

// Scan for Nearby Beacons
void scanForBeacons(std::vector<BLEAdvertisedDevice> &devices) {
  BLEScan *scanner = BLEDevice::getScan();
  scanner->setActiveScan(true);
  BLEScanResults results = scanner->start(5, false);

  for (int i = 0; i < results.getCount(); i++) {
    devices.push_back(results.getDevice(i));
  }
}

// UI to Select Cloning Target
void selectCloningTarget(std::vector<BLEAdvertisedDevice> &devices) {
  int index = 0;
  bool confirmed = false;

  while (!confirmed) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Select Target:");
    for (int i = 0; i < devices.size(); i++) {
      if (i == index) display.print("> ");
      else display.print("  ");
      display.println(devices[i].getAddress().toString().c_str());
    }
    display.display();

    if (digitalRead(BUTTON_UP) == LOW) {
      index = (index - 1 + devices.size()) % devices.size();
      delay(200);
    } else if (digitalRead(BUTTON_DOWN) == LOW) {
      index = (index + 1) % devices.size();
      delay(200);
    } else if (digitalRead(BUTTON_SELECT) == LOW) {
      spoofedUUID = devices[index].getServiceUUID().toString().c_str();
      confirmed = true;
    }
  }
}

void BeaconSpoofer::start() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Starting Spoofer...");
  display.display();

  // Advertise multiple devices
  BLEAdvertising *advertisers[numberOfDevices];
  for (int i = 0; i < numberOfDevices; i++) {
    advertisers[i] = BLEDevice::getAdvertising();
    advertisers[i]->addServiceUUID(spoofedUUID);
    advertisers[i]->setMinInterval(advertisingInterval);
    advertisers[i]->setMaxInterval(advertisingInterval + 50);
    advertisers[i]->start();
    delay(10);  // Prevent stack overflow
  }

  // Display Spoofing Status
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Spoofing Devices:");
  for (int i = 0; i < numberOfDevices; i++) {
    display.setCursor(0, 10 + (i * 10));
    display.print("Device ");
    display.print(i + 1);
    display.print(": ");
    display.println(spoofedUUID.c_str());
  }
  display.display();

  Serial.println("Spoofer Running.");
}

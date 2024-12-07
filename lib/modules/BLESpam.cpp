#include "BLESpam.h"
#include <BLEDevice.h>
#include <vector>

// Configuration Variables
int numAdvertisements = 10;             // Number of advertisements to cycle through
int advertisingInterval = 50;           // Advertising interval in milliseconds
std::vector<String> advertisingData;    // List of random advertising UUIDs

void BLESpam::start() {
  Serial.println("Starting BLE Spam...");

  // Initialize advertising data
  initializeAdvertisingData();

  // Create and start BLE advertising
  BLEAdvertising *advertiser = BLEDevice::getAdvertising();
  advertiser->setMinInterval(advertisingInterval);
  advertiser->setMaxInterval(advertisingInterval + 10);

  // Advertise multiple data sets in a loop
  for (int i = 0; i < numAdvertisements; i++) {
    advertiser->addServiceUUID(advertisingData[i % advertisingData.size()]);
    advertiser->addManufacturerData("BLE_Spam_Test");
    advertiser->start();
    Serial.println("Advertising: " + advertisingData[i % advertisingData.size()]);
    delay(advertisingInterval);  // Delay between advertisements
    advertiser->stop();          // Stop the current advertisement
  }

  Serial.println("BLE Spam Complete.");
}

void BLESpam::initializeAdvertisingData() {
  advertisingData.clear();
  for (int i = 0; i < numAdvertisements; i++) {
    char uuid[37];
    snprintf(uuid, sizeof(uuid), "%08X-%04X-%04X-%04X-%012X",
             random(0x100000000), random(0x10000), random(0x10000),
             random(0x10000), random(0x100000000000));
    advertisingData.push_back(String(uuid));
  }
  Serial.println("Advertising Data Initialized.");
}

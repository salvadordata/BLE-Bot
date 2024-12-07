#include "BLESpam.h"
#include <BLEDevice.h>

void BLESpam::start() {
  Serial.println("Starting BLE Spam...");
  
  BLEAdvertising *advertiser = BLEDevice::getAdvertising();
  advertiser->addServiceUUID("12345678-1234-1234-1234-123456789abc"); // Example UUID
  advertiser->setMinInterval(0x20);  // Minimum interval
  advertiser->setMaxInterval(0x40);  // Maximum interval
  advertiser->start();

  Serial.println("BLE Spam active.");
}
#include "ProximityTracker.h"
#include <BLEDevice.h>

void ProximityTracker::start() {
  Serial.println("Starting Proximity Tracker...");

  BLEScan *scanner = BLEDevice::getScan();
  scanner->setActiveScan(true);
  BLEScanResults results = scanner->start(5, false);

  for (int i = 0; i < results.getCount(); i++) {
    BLEAdvertisedDevice device = results.getDevice(i);
    Serial.println("Device: " + device.getAddress().toString() +
                   ", RSSI: " + String(device.getRSSI()));
  }

  Serial.println("Proximity Tracker complete.");
}

#ifndef BLESPAM_H
#define BLESPAM_H

#include <string>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

class BLESpam {
public:
    // Initialize the BLE Spam system
    static void start(const std::string& deviceName = "BLESpam");

private:
    static BLEServer* pServer;               // BLE Server instance
    static BLEAdvertising* pAdvertising;    // BLE Advertising instance

    // Internal methods
    static void initializeBLE(const std::string& deviceName);  // Initializes BLE
    static void initializeAdvertisingData();                   // Sets up advertising data
    static void startAdvertising();                            // Starts advertising
};

#endif // BLESPAM_H

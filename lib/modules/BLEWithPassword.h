#ifndef BLEWITHPASSWORD_H
#define BLEWITHPASSWORD_H

#include <iostream> // For debugging purposes

class BLEWithPassword {
public:
  static void start();  // Start BLE module with password

private:
  static void initializeAdvertisingData();  // Initialize BLE advertising data
};

#endif


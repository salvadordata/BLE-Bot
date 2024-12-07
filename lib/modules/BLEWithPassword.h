#ifndef BLEWITHPASSWORD_H
#define BLEWITHPASSWORD_H

#include <Arduino.h>

class BLEWithPassword {
public:
  static void start();  // Start BLE with Password
  static void logPassword(const String &password);  // Log password to SD card
};

#endif

#ifndef BLESPAM_H
#define BLESPAM_H

class BLESpam {
public:
  static void start();  // Start BLE Spam

private:
  static void initializeAdvertisingData();  // Initialize random advertising data
};

#endif
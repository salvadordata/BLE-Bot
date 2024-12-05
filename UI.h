#ifndef UI_H
#define UI_H

#include <vector>
#include <BLEAdvertisedDevice.h>

class UI {
public:
  // Initialize the OLED display and buttons
  static void init();

  // Show the main menu
  static void showMenu();

  // Handle button inputs for menu navigation
  static void handleButtonInput();

  // Execute the selected module
  static void executeModule(int menuIndex);

private:
  // Helper functions for specific module UI features
  static void scanForBeacons(std::vector<BLEAdvertisedDevice> &devices); // Scan for nearby BLE devices
  static void selectCloningTarget(std::vector<BLEAdvertisedDevice> &devices); // Select a device for cloning
};

#endif

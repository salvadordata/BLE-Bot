#ifndef UI_H
#define UI_H

#include <Arduino.h>
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

  // Execute the selected module based on menuIndex
  static void executeModule(int menuIndex);

private:
  // Dynamic configuration menu
  static void configureBLE();

  // Helper functions for input handling
  static String inputString(const String &label);  // Handle string input
  static int inputInt(const String &label);        // Handle integer input
};

#endif

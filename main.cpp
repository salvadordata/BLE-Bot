#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <BLEDevice.h>

// Include UI and all modules
#include "UI.h"
#include "modules/BLESpam.h"
#include "modules/BLEWithPassword.h"
#include "modules/BLESniffer.h"
#include "modules/HIDEmulation.h"
#include "modules/PairingLogger.h"
#include "modules/BLEMITM.h"
#include "modules/ProximityTracker.h"
#include "modules/BeaconSpoofer.h"
#include "modules/DDoSAttack.h"

// OLED Display Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Function to show the animated intro
void showIntro() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Step 1: "BLE Bot" Text Drop Animation
  for (int y = -10; y <= 16; y += 2) {
    display.clearDisplay();
    display.setCursor(20, y);
    display.print("BLE Bot");
    display.display();
    delay(50);
  }

  // Step 2: "by SalvadorData" Fades In
  for (int brightness = 0; brightness <= 255; brightness += 15) {
    display.clearDisplay();
    display.setCursor(20, 16);
    display.print("BLE Bot");
    display.setCursor(10, 32);
    display.print("by SalvadorData");
    display.display();
    delay(50);
  }

  delay(500);

  // Step 3: Draw Robot Face
  for (int x = 40; x < 88; x++) {
    display.clearDisplay();
    display.setCursor(20, 16);
    display.print("BLE Bot");
    display.setCursor(10, 32);
    display.print("by Salvador Data");

    // Draw Robot Face
    display.drawRect(40, 40, 48, 16, SSD1306_WHITE);  // Head
    display.drawCircle(50, 48, 3, SSD1306_WHITE);     // Left Eye
    display.drawCircle(70, 48, 3, SSD1306_WHITE);     // Right Eye
    display.drawLine(50, 55, 70, 55, SSD1306_WHITE);  // Mouth

    display.display();
    delay(20);
  }

  delay(1000);

  // Step 4: Transition to Main Menu
  for (int i = 0; i < SCREEN_WIDTH; i += 4) {
    display.clearDisplay();
    display.setCursor(20, 16);
    display.print("BLE Bot");
    display.setCursor(10, 32);
    display.print("by Salvador Data");
    display.fillRect(i, 0, 4, SCREEN_HEIGHT, SSD1306_BLACK);  // Wipe screen
    display.display();
    delay(30);
  }
}

// Setup function
void setup() {
  Serial.begin(115200);

  // Initialize OLED
  if (!display.begin(SSD1306_I2C_ADDRESS, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
  display.clearDisplay();
  display.display();

  // Show the Intro Animation
  showIntro();

  // Initialize BLE
  BLEDevice::init("BLE Multi-Tool");

  // Initialize UI and show the menu
  UI::init();
  UI::showMenu();
}

// Loop function
void loop() {
  // Handle button inputs and module execution via the UI
  UI::handleButtonInput();
}

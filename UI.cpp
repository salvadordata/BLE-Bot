#include "UI.h"
#include "Adafruit_SSD1306.h"
#include "modules/BLESpam.h"
#include "modules/BLEWithPassword.h"
#include "modules/BLESniffer.h"
#include "modules/HIDEmulation.h"
#include "modules/PairingLogger.h"
#include "modules/BLEMITM.h"
#include "modules/ProximityTracker.h"
#include "modules/BeaconSpoofer.h"

// OLED Display Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Buttons
#define BUTTON_UP 2
#define BUTTON_DOWN 3
#define BUTTON_SELECT 4

// Menu Items
String menuItems[] = {
  "BLE Spam",
  "BLE w/ Password",
  "Sniffer",
  "HID Emulation",
  "Pairing Logger",
  "MITM",
  "Proximity Tracker",
  "Beacon Spoofer",
  "Clone Beacon"
};
int totalMenuItems = sizeof(menuItems) / sizeof(menuItems[0]);
int currentMenu = 0;

// HID Emulation Payloads
String payloads[] = {
  "Hello World!",
  "Ctrl+Alt+Del",
  "Open Notepad",
  "Execute Script"
};
int totalPayloads = sizeof(payloads) / sizeof(payloads[0]);
int selectedPayload = 0;

void UI::init() {
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);

  if (!display.begin(SSD1306_I2C_ADDRESS, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;)
      ;
  }
  display.clearDisplay();
  display.display();
}

void UI::showMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  for (int i = 0; i < totalMenuItems; i++) {
    if (i == currentMenu) {
      display.setCursor(0, i * 10);
      display.print("> ");
    } else {
      display.setCursor(10, i * 10);
    }
    display.println(menuItems[i]);
  }
  display.display();
}

void UI::handleButtonInput() {
  if (digitalRead(BUTTON_UP) == LOW) {
    currentMenu = (currentMenu - 1 + totalMenuItems) % totalMenuItems;
    showMenu();
    delay(200);  // Debounce
  }
  if (digitalRead(BUTTON_DOWN) == LOW) {
    currentMenu = (currentMenu + 1) % totalMenuItems;
    showMenu();
    delay(200);
  }
  if (digitalRead(BUTTON_SELECT) == LOW) {
    executeModule(currentMenu);
    delay(200);
  }
}

void UI::executeModule(int menuIndex) {
  switch (menuIndex) {
    case 0: BLESpam::start(); break;
    case 1: BLEWithPassword::start(); break;
    case 2: BLESniffer::start(); break;
    case 3:  // HID Emulation Integration
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Select Payload:");
      for (int i = 0; i < totalPayloads; i++) {
        if (i == selectedPayload) display.print("> ");
        else display.print("  ");
        display.println(payloads[i]);
      }
      display.display();

      if (digitalRead(BUTTON_UP) == LOW) {
        selectedPayload = (selectedPayload - 1 + totalPayloads) % totalPayloads;
        delay(200);
      } else if (digitalRead(BUTTON_DOWN) == LOW) {
        selectedPayload = (selectedPayload + 1) % totalPayloads;
        delay(200);
      } else if (digitalRead(BUTTON_SELECT) == LOW) {
        HIDEmulation::start();
      }
      break;
    case 4: PairingLogger::start(); break;
    case 5: BLEMITM::start(); break;
    case 6: ProximityTracker::start(); break;
    case 7: BeaconSpoofer::start(); break;
    case 8:  // Clone Beacon
      std::vector<BLEAdvertisedDevice> devices;
      scanForBeacons(devices);
      selectCloningTarget(devices);
      break;
  }
}

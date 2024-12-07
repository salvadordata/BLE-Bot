#include "HIDEmulation.h"
#include <BLEDevice.h>
#include <BLEHIDDevice.h>
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;

// Current Mode
enum HIDMode { KEYBOARD_MODE, MOUSE_MODE };
HIDMode currentMode = KEYBOARD_MODE;

// Keyboard Payloads
String payloads[] = {
  "Hello World!",
  "Ctrl+Alt+Del",
  "Open Notepad",
  "Execute Script"
};
int totalPayloads = sizeof(payloads) / sizeof(payloads[0]);
int selectedPayload = 0;

// Mouse Position
int mouseX = 0;
int mouseY = 0;

// Initialize HID Device
BLEHIDDevice *hid;
BLECharacteristic *input;

// Helper Functions
void executeKeyboardPayload() {
  String payload = payloads[selectedPayload];

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Executing: ");
  display.println(payload);
  display.display();

  if (payload == "Hello World!") {
    input->setValue((uint8_t *)"Hello World!", 12);
  } else if (payload == "Ctrl+Alt+Del") {
    // Send Ctrl+Alt+Del
    uint8_t ctrlAltDel[] = {0x01, 0x04, 0x07};
    input->setValue(ctrlAltDel, 3);
  }
  // Add more payloads as needed

  input->notify();
}

void moveMouse() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Mouse Mode: ");
  display.setCursor(0, 10);
  display.print("X: ");
  display.print(mouseX);
  display.print(" Y: ");
  display.print(mouseY);
  display.display();

  uint8_t mouseData[] = {mouseX, mouseY, 0};  // X, Y, Button
  input->setValue(mouseData, sizeof(mouseData));
  input->notify();
}

// HID Emulation Main Function
void HIDEmulation::start() {
  BLEDevice::init("HID Emulator");
  hid = BLEDevice::createHIDDevice();
  input = hid->inputReport(1);

  hid->manufacturer()->setValue("BLE Multi-Tool");
  hid->pnp(0x02, 0xe502, 0xa111, 0x0210);  // HID descriptor
  hid->startServices();
  BLEDevice::startAdvertising();

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("HID Emulation");
  display.display();

  // Main HID Loop
  while (true) {
    if (currentMode == KEYBOARD_MODE) {
      executeKeyboardPayload();
    } else if (currentMode == MOUSE_MODE) {
      moveMouse();
    }
    delay(100);
  }
}

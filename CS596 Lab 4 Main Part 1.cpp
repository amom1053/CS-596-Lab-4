#include <Arduino.h>
#include <BLEDevice.h>

#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
 
// Defining LED pin
const int pin = 12;
// Initializing variable to hold 'off' state at start of program
bool state = false;

class MyCallbacks: public BLECharacteristicCallbacks {
   void onWrite(BLECharacteristic *pCharacteristic) {
     std::string value = pCharacteristic->getValue();
 
     if (value.length() > 0) {
      Serial.print("Received: ");
      for (int i = 0; i < value.length(); i++)
        Serial.print(value[i]);
        if (value == "0") {
          digitalWrite(pin, LOW);
          state = false;
          Serial.println(", The LED is off");
        }
        else if (value == "1") {
          digitalWrite(pin, HIGH);
          state = true;
          Serial.println(", The LED is on");
        }
        else {
          Serial.println(", That was not a valid command!");
        }
     }
     Serial.println();
   }
};
 
void setup() {
 Serial.begin(9600);
 
 Serial.println("1- Download and install an BLE scanner app in your phone");
 Serial.println("2- Scan for BLE devices in the app");
 Serial.println("3- Connect to MyESP32");
 Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write '0' or '1' to turn the LED on or off");
 Serial.println("5- See the magic =)");

 // Configuring the pin for our blue LED as an output variable
 pinMode(pin, OUTPUT);
 
 BLEDevice::init("A&ASDSUCS");
 BLEServer *pServer = BLEDevice::createServer();
 
 BLEService *pService = pServer->createService(SERVICE_UUID);
 
 BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                        CHARACTERISTIC_UUID,
                                        BLECharacteristic::PROPERTY_READ |
                                        BLECharacteristic::PROPERTY_WRITE
                                      );
 
 pCharacteristic->setCallbacks(new MyCallbacks());
 
 pCharacteristic->setValue("Hello World");
 pService->start();
 
 BLEAdvertising *pAdvertising = pServer->getAdvertising();
 pAdvertising->start();
}
 
void loop() {
 delay(2000);
}



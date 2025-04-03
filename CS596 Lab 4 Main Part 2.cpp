#include "SparkFunLSM6DSO.h"
#include "Wire.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


// Creating LSM6DSO object
LSM6DSO myIMU;


#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"


// Server set-up code
// Creating pointer to the server object
BLEServer *pServer = NULL;
// Creating pointer to characteristic object
BLECharacteristic *pCharacteristic = NULL;


// Creating counter variable for step count
int steps = 0;
// Creating threschold variable to ensure it counts prooper step movement
float threshold = 1.5;
// bool used to capture if movement was a valid step
bool valid = false;




void setup() {
  Serial.begin(9600);


  Wire.begin();
  delay(10);


  if (myIMU.begin())
    Serial.println("Ready.");
  else {
    Serial.println("Could not connect to IMU.");
    Serial.println("Freezing");
  }


  if (myIMU.initialize(BASIC_SETTINGS)) {
    Serial.println("Loaded Settings.");
  }


  Serial.println("Calibrating sensor...");
  delay(5000);


  BLEDevice::init("A&ASDSUCS");
  pServer = BLEDevice::createServer();


  BLEService* pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                    );


// Setting initial value to be sent to the app (this will increment as steps are taken)
  pCharacteristic->setValue("Steps: 0");


  pService->start();


  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Start stepping :)");
}






void loop() {
  // Creating x and y axis variables to read x and y acceleration
  float xAxis = myIMU.readFloatAccelX();
  float yAxis = myIMU.readFloatAccelY();


  // calculating the root mean formula using input of x and y axis values
  float rootMeanFormula = sqrt(xAxis * xAxis + yAxis * yAxis);


  // Determines if the movement made was a valid 'step'
  if (rootMeanFormula > threshold) {
    if (!valid) {
      // If so, increment step counter and print new number of steps
      steps++;
      Serial.print("Steps: ");
      Serial.println(steps);
      valid = true;
    }
  }
  else {
    valid = false;
  }


// Create a string to hold step count value and print out to the nRF Connect App Server
String stringOutput = "Steps: " + String(steps);
pCharacteristic->setValue(stringOutput.c_str());


delay(20);
}

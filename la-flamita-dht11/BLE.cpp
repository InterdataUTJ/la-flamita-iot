#include "BLE.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "d8677220-5a9f-4392-9184-b64e75a6ff07"
#define CHARACTERISTIC_UUID "95fa44cc-5184-42a5-a9ff-e098de1326d8"
BLE::BLE() {}

void BLE::server(String deviceName, void (*callback)()){
  BLEDevice::init(deviceName);
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );

  pCharacteristic->setCallbacks(new MyCallbacks(callback));

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}



void BLE::client(String deviceName){

  BLEDevice::init(deviceName);
  pClient = BLEDevice::createClient();
  BLEScan* pScan = BLEDevice::getScan();
  pScan->setActiveScan(true);
  BLEScanResults* foundDevices = pScan->start(5);
  
  for (int i = 0; i < foundDevices->getCount(); i++) {
      BLEAdvertisedDevice device = foundDevices->getDevice(i);
      if (device.haveServiceUUID() && device.getServiceUUID().equals(BLEUUID(SERVICE_UUID))) {
          pClient->connect(&device);
          BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
          if (pRemoteService) {
              pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
          }
          break;
      }
  }

}


void BLE::send(String msg) {
  if (pRemoteCharacteristic) {
    pRemoteCharacteristic->writeValue(msg.c_str(), msg.length());
  }
}
#ifndef BLE_H
#define BLE_H
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

class BLE {
  private:
    BLECharacteristic *pCharacteristic;
    BLEClient* pClient;
    BLERemoteCharacteristic* pRemoteCharacteristic;

  public: 
    BLE();
    void server(String deviceName, void (*callback)());
    void client(String deviceName);
    void send(String msg);
};


class MyCallbacks : public BLECharacteristicCallbacks {
  void (*callback)();

  public:
    MyCallbacks(void (*cb)()) : callback(cb) {}
    void onWrite(BLECharacteristic *pCharacteristic) override {
        callback();
    }
};

#endif
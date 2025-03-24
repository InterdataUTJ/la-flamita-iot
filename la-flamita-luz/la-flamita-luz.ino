#include <Arduino.h>
#include <ArduinoJson.h>
#include <TickTwo.h>
#include "Send.h"
#include "Configurator.h"
#include "BLE.h"

// BLE
BLE bleClient;


// Sensor CONFIG
#define SENSOR_PIN 14
#define SENSOR_CONFIG_SLOT 0
#define SENSOR_TIMES_ALARM 6 // Times open to activate alarm
Send sensorSender;
unsigned int timesOpen = 0;


// Internal config
#define SENDER_DELAY 5000
Configurator config;
unsigned long nextSend = 0;


// TImers
void senderTimerCallback();
TickTwo senderTimer(senderTimerCallback, SENDER_DELAY);




// Get the info to send
JsonDocument getData() {
  JsonDocument root;
  JsonDocument dato;
  dato["Estado"] = (digitalRead(SENSOR_PIN) == HIGH ? "1" : "0");

  if (dato["Estado"] == "1") timesOpen++;
  else timesOpen = 0;

  root["dato"] = dato;
  return root;
}



void senderTimerCallback() {
  if (config.canSend(SENSOR_CONFIG_SLOT)) {
    int state = sensorSender.send(getData());
    if (state == 2) config.askConfig();
  }

  if (timesOpen >= SENSOR_TIMES_ALARM) {
    bleClient.send("[BLE] Turn ON alarm");
  }
}



void setup() {
  // Internal config
  pinMode(2, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);
  config.begin();
  sensorSender.setApi(config.getApiUrl(), config.getApiKey(SENSOR_CONFIG_SLOT), config.getApiId(SENSOR_CONFIG_SLOT));

  // Start timers
  senderTimer.start();
  bleClient.client("ESP32-LUZ-BLE-CLIENT");
}


void loop() {
  // Setup new API details if configuration changed
  if (config.step()) {
    sensorSender.setApi(config.getApiUrl(), config.getApiKey(SENSOR_CONFIG_SLOT), config.getApiId(SENSOR_CONFIG_SLOT));
  }

  // Update timers
  senderTimer.update();
}
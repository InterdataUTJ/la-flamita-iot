#include <Arduino.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include "Send.h"
#include "Configurator.h"

// Sensor CONFIG
#define DHT_PIN 14
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);


// Internal config
#define SENDER_DELAY 5000
Configurator config;
Send sender;
unsigned long nextSend = 0;


// Get the info to send
JsonDocument getData() {
  JsonDocument jsonData;
  jsonData["Temperatura"] = dht.readTemperature();
  jsonData["Humedad"] = dht.readHumidity();
  return jsonData;
}


void setup() {
  // Sensor config
  dht.begin();

  // Internal config
  pinMode(2, OUTPUT);
  config.begin(1);
  sender.setApi(config.getApiUrl(), config.getApiKey());
}


void loop() {
  // Setup new API details if configuration changed
  if (config.step()) {
    sender.setApi(config.getApiUrl(), config.getApiKey());
  }

  unsigned long currentTime = millis();
  if (currentTime >= nextSend && config.canSend()) {
    
    // Send data
    int state = sender.send(getData());
    nextSend = currentTime + SENDER_DELAY;

    if (state == 2) config.askConfig();
  }
}
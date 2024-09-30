#include <Arduino.h>
#include <DHT.h>
#include "Send.h"
#include "Configurator.h"

#define DHT_PIN 4
#define DHT_TYPE DHT11
#define SENDER_DELAY 5000

Configurator config;
Send sender;
DHT dht(DHT_PIN, DHT11);
unsigned long nextSend = 0;



void setup() {
  dht.begin();
  if (config.begin()) {
    sender.setApi(config.getApiServer(), config.getApiKey());
  }
}


void loop() {
  // Setup new API details if configuration changed
  if (config.step()) {
    sender.setApi(config.getApiServer(), config.getApiKey());
  }
  
  unsigned long currentTime = millis();
  if (currentTime >= nextSend && !config.needConfig()) {
    int state = sender.data(dht.readHumidity(), dht.readTemperature());
    nextSend = currentTime + SENDER_DELAY;

    if (state == 2) {
      config.askConfig();
    }
  }
}
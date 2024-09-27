#include <Arduino.h>
#include <DHT.h>
#include "secrets.h"
#include "Send.h"
#include "Configurator.h"

#define DHT_PIN 4
#define DHT_TYPE DHT11
#define SENDER_DELAY 5000


Send sender(SECRET_SERVER);
DHT dht(DHT_PIN, DHT11);
Configurator config;

unsigned long nextSend = 0;



void setup() {
  dht.begin();
  config.begin();
}


void loop() {
  config.step();
  unsigned long currentTime = millis();

  if (currentTime >= nextSend) {
    sender.data(dht.readHumidity(), dht.readTemperature());
    nextSend = currentTime + SENDER_DELAY;
  }
}
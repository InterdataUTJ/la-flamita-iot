#include <Arduino.h>
#include <DHT.h>
#include "Send.h"
#include "Configurator.h"

#define RED_LED_PIN   25
#define GREEN_LED_PIN 26
#define BLUE_LED_PIN  27
#define RGB_LED_COMMON false // Catodo

#define DHT_PIN 14
#define DHT_TYPE DHT11
#define SENDER_DELAY 5000

Configurator config;
Send sender;
DHT dht(DHT_PIN, DHT_TYPE);
unsigned long nextSend = 0;



void setup() {
  dht.begin();
  if (config.begin(RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN, RGB_LED_COMMON)) {
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
#include "Configurator.h"
#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#include <RGBLed.h>

#define WIFI_TIMEOUT 10000

#define PREFERENCES_PARTITION "la-flamita-iot"

#define RED_LED_PIN   18
#define GREEN_LED_PIN 19
#define BLUE_LED_PIN  21

Configurator::Configurator() {
  configMode = false;
  wifiConnected = false;
  Preferences pref;
  API_KEY = "";
  API_SERVER = "";
  WIFI_SSID = "";
  WIFI_PASS = "";
}

void Configurator::begin() {
  // Setup mode
  rgb = new RGBLed(RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN, RGBLed::COMMON_CATHODE);
  Serial.begin(115200);
  rgb->brightness(RGBLed::YELLOW, 100);

  pref.begin(PREFERENCES_PARTITION);
  WIFI_SSID = pref.getString("WIFI_SSID", "");
  WIFI_PASS = pref.getString("WIFI_PASS", "");
  API_KEY = pref.getString("API_KEY", "");
  API_SERVER = pref.getString("API_SERVER", "");

  if (!checkConfig()) {
    rgb->brightness(RGBLed::RED, 100);
  } else {
    rgb->brightness(RGBLed::BLUE, 10);
  }
}


bool Configurator::checkConfig() {
  Serial.print("WIFI_SSID:");
  Serial.println(WIFI_SSID);

  Serial.print("WIFI_PASS:");
  Serial.println(WIFI_PASS);

  Serial.print("API_KEY:");
  Serial.println(API_KEY);

  Serial.print("API_SERVER:");
  Serial.println(API_SERVER);

  if (WIFI_PASS == "" || WIFI_SSID == "" || API_KEY == "" || API_SERVER == "") {
    return false;
  } else {
    unsigned long time = millis();
    Serial.println("Attemping connect wifi...");
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while(WiFi.status() != WL_CONNECTED) {
      if (millis() >= (time + WIFI_TIMEOUT)) {
        Serial.println("Timeout, couldnt connect...");
        return false;
      }
    }

    wifiConnected = (WiFi.status() == WL_CONNECTED);
    return wifiConnected;
  }
}


void Configurator::config() {
  if (configMode) rgb->brightness(RGBLed::YELLOW, 100);
  while(configMode) {
    while (Serial.available()) {
      String result = Serial.readString();

      if (result.length() > 5 && result.substring(0, 5) == "SSID?") {
        WIFI_SSID = result.substring(5);
        pref.putString("WIFI_SSID", WIFI_SSID);
        Serial.print("WIFI_SSID UPDATED: ");
        Serial.println(WIFI_SSID);
        
      } else if (result.length() > 5 && result.substring(0, 5) == "PASS?") {
        WIFI_PASS = result.substring(5);
        pref.putString("WIFI_PASS", WIFI_PASS);
        Serial.print("WIFI_PASS UPDATED: ");
        Serial.println(WIFI_PASS);

      } else if (result.length() > 7 && result.substring(0, 7) == "APIKEY?") {
        API_KEY = result.substring(7);
        pref.putString("API_KEY", API_KEY);
        Serial.print("API_KEY UPDATED: ");
        Serial.println(API_KEY);

      } else if (result.length() > 10 && result.substring(0, 10) == "APISERVER?") {
        API_SERVER = result.substring(10);
        pref.putString("API_SERVER", API_SERVER);
        Serial.print("API_SERVER UPDATED: ");
        Serial.println(API_SERVER);

      }  else if (result == "STOP_CONFIG_MODE") {
        configMode = false;
        Serial.println("STOPING CONFIG MODE...");
      }
    }
  }

  if (!checkConfig()) {
    rgb->brightness(RGBLed::RED, 100);
  } else {
    rgb->brightness(RGBLed::BLUE, 10);
  }
}

void Configurator::step() {
  while (Serial.available()) {
    String result = Serial.readString();

    if (result.indexOf("START_CONFIG_MODE") >= 0) {
      configMode = true;
      config();
    }
  }
}
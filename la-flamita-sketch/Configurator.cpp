#include "Configurator.h"
#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#include <RGBLed.h>

#define WIFI_TIMEOUT 10000

#define PREFERENCES_PARTITION "la-flamita-iot"

#define DEFAULT_RED_LED_PIN   18
#define DEFAULT_GREEN_LED_PIN 19
#define DEFAULT_BLUE_LED_PIN  21

Configurator::Configurator() {
  configNeeded = false;
  configMode = false;
  wifiConnected = false;
  Preferences pref;
  API_KEY = "";
  API_SERVER = "";
  WIFI_SSID = "";
  WIFI_PASS = "";
}

bool Configurator::begin() {
  // Setup mode
  Serial.begin(115200);
  Serial.println("[SYSTEM] Starting system.");
  rgb = new RGBLed(DEFAULT_RED_LED_PIN, DEFAULT_GREEN_LED_PIN, DEFAULT_BLUE_LED_PIN, RGBLed::COMMON_CATHODE);
  rgb->brightness(RGBLed::YELLOW, 100);

  pref.begin(PREFERENCES_PARTITION);
  WIFI_SSID = pref.getString("WIFI_SSID", "");
  WIFI_PASS = pref.getString("WIFI_PASS", "");
  API_KEY = pref.getString("API_KEY", "");
  API_SERVER = pref.getString("API_SERVER", "");

  return checkConfig();
}

bool Configurator::begin(int redPin, int greenPin, int bluePin, bool common) {
  // Setup mode
  Serial.begin(115200);
  Serial.println("[SYSTEM] Starting system.");
  rgb = new RGBLed(redPin, greenPin, bluePin, common);
  rgb->brightness(RGBLed::YELLOW, 100);

  pref.begin(PREFERENCES_PARTITION);
  WIFI_SSID = pref.getString("WIFI_SSID", "");
  WIFI_PASS = pref.getString("WIFI_PASS", "");
  API_KEY = pref.getString("API_KEY", "");
  API_SERVER = pref.getString("API_SERVER", "");

  return checkConfig();
}


bool Configurator::checkConfig() {
  if (WIFI_PASS == "" || WIFI_SSID == "" || API_KEY == "" || API_SERVER == "") {
    Serial.println("[CONFIGURATOR] Configuration error. Config needed.");
    rgb->brightness(RGBLed::RED, 100);
    configNeeded = true;
    return false;

  } else {
    unsigned long time = millis();
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while(WiFi.status() != WL_CONNECTED) {
      if (millis() >= (time + WIFI_TIMEOUT)) {
        Serial.println("[CONFIGURATOR] Timeout, couldnt connect to WiFi.");
        return false;
      }
    }

    wifiConnected = (WiFi.status() == WL_CONNECTED);

    if (wifiConnected) {
      Serial.println("[CONFIGURATOR] Configuration successful.");
      rgb->brightness(RGBLed::BLUE, 10);
    } else {
      Serial.println("[CONFIGURATOR] Configuration error. Config needed.");
      rgb->brightness(RGBLed::RED, 100);
    }

    configNeeded = !wifiConnected;
    return wifiConnected;
  }
}


bool Configurator::config() {
  if (configMode) rgb->brightness(RGBLed::YELLOW, 100);
  while(configMode) {
    while (Serial.available()) {
      String result = Serial.readString();

      if (result.length() > 5 && result.substring(0, 5) == "SSID?") {
        WIFI_SSID = result.substring(5);
        pref.putString("WIFI_SSID", WIFI_SSID);
        Serial.print("[CONFIGURATOR] WIFI_SSID updated: ");
        Serial.println(WIFI_SSID);
        
      } else if (result.length() > 5 && result.substring(0, 5) == "PASS?") {
        WIFI_PASS = result.substring(5);
        pref.putString("WIFI_PASS", WIFI_PASS);
        Serial.print("[CONFIGURATOR] WIFI_PASS updated: ");
        Serial.println(WIFI_PASS);

      } else if (result.length() > 7 && result.substring(0, 7) == "APIKEY?") {
        API_KEY = result.substring(7);
        pref.putString("API_KEY", API_KEY);
        Serial.print("[CONFIGURATOR] API_KEY updated: ");
        Serial.println(API_KEY);

      } else if (result.length() > 10 && result.substring(0, 10) == "APISERVER?") {
        API_SERVER = result.substring(10);
        pref.putString("API_SERVER", API_SERVER);
        Serial.print("[CONFIGURATOR] API_SERVER updated: ");
        Serial.println(API_SERVER);

      } else if (result == "CLEAR_CONFIG") {
        pref.clear();
        WIFI_SSID = "";
        WIFI_PASS = "";
        API_SERVER = "";
        API_KEY = "";
        Serial.println("[CONFIGURATOR] Config cleared.");
        
      } else if (result == "STOP_CONFIG_MODE") {
        configMode = false;
        Serial.println("[CONFIGURATOR] Stoping config mode.");
      }
    }
  }

  return checkConfig();
}

bool Configurator::step() {
  while (Serial.available()) {
    String result = Serial.readString();

    if (result.indexOf("START_CONFIG_MODE") >= 0) {
      Serial.println("[CONFIGURATOR] Configuration mode started.");
      configMode = true;
      return config();
    }
  }

  return false;
}

void Configurator::askConfig() {
  rgb->brightness(RGBLed::RED, 100);
  configNeeded = true;
  Serial.println("[CONFIGURATOR] Configuration needed.");
}

bool Configurator::needConfig() {
  return configNeeded;
}

String Configurator::getApiKey() {
  return API_KEY;
}

String Configurator::getApiServer() {
  return API_SERVER;
}
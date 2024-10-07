#include <memory>
#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#include <RGBLed.h>
  
class Configurator {
  private:
    bool configNeeded;
    bool configMode;
    bool wifiConnected;
    RGBLed* rgb;;
    Preferences pref;
    String API_KEY;
    String API_SERVER;
    String WIFI_SSID;
    String WIFI_PASS;

    bool config();
    bool checkConfig();

  public: 
    Configurator();
    bool begin();
    bool begin(int redPin, int greenPin, int bluePin, bool common);
    bool step();
    bool needConfig();
    void askConfig();
    String getApiKey();
    String getApiServer();
};

#endif
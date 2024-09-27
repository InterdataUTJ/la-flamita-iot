#include <memory>
#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#include <RGBLed.h>
  
class Configurator {
  private:
    bool configMode;
    bool wifiConnected;
    RGBLed* rgb;;
    Preferences pref;
    String API_KEY;
    String API_SERVER;
    String WIFI_SSID;
    String WIFI_PASS;

    void config();
    bool checkConfig();

  public: 
    Configurator();
    void begin();
    void step();
};

#endif
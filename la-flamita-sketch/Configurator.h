#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H
#include <Preferences.h>
  
class Configurator {
  private:
    bool needConfig; 
    Preferences pref;
    String API_URL;
    String WIFI_SSID;
    String WIFI_PASS;
    JsonDocument API_DEVICES;

    bool checkConfig();
    void prompt();

  public: 
    Configurator();
    bool begin();
    bool step();
    bool canSend();
    void askConfig();
    String getApiUrl();
    String getApiId(String device);
    String getApiKey(String device);
};

#endif
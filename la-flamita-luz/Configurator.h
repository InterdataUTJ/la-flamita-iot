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
    String API_KEY[3];
    String API_ID[3];

    bool checkConfig();
    void prompt();

  public: 
    Configurator();
    bool begin();
    bool step();
    bool canSend(int slot);
    void askConfig();
    String getApiUrl();
    String getApiId(int slot);
    String getApiKey(int slot);
};

#endif
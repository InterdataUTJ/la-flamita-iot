#ifndef SEND_H
#define SEND_H

#include <Arduino.h>
  
class Send {
  private:
    short FAILED_COUNTER = 0;
    String API_SERVER;
    String API_KEY;
    String json(float humidity, float tempeture);

  public: 
    Send();
    Send(String newAPiServer, String newApiKey);
    void setApiServer(String newApiServer);
    void setApiKey(String newApiKey);
    void setApi(String newApiServer, String newApiKey);
    int data(float humidity, float tempeture);
};

#endif
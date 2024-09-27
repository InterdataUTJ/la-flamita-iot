#ifndef SEND_H
#define SEND_H

#include <Arduino.h>
  
class Send {
  private:
    String url;
    String json(float humidity, float tempeture);

  public: 
    Send(String new_url);
    void setUrl(String new_url);
    bool data(float humidity, float tempeture);
};

#endif
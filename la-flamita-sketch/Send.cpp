#include "Send.h"
#include <HTTPClient.h>
#include <WiFi.h>
#include <Arduino.h>

Send::Send(String new_url) {
  url = new_url;
}

void Send::setUrl(String new_url) {
  url = new_url;
}

String Send::json(float humidity, float tempeture) {
  String data = "{\"humidity\": ${H},\"tempeture\": ${T}}";
  data.replace("${H}", String(humidity));
  data.replace("${T}", String(tempeture));
  return data;
}

bool Send::data(float humidity, float tempeture) {
  if (WiFi.status() != WL_CONNECTED) return false;
  
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(json(humidity, tempeture));
  http.end();
      
  if (httpResponseCode >= 200 && httpResponseCode <= 299) {
    Serial.println("Data sent succesfully.");
    return true;
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
      
  return false;
}
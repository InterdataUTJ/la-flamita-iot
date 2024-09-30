#include "Send.h"
#include <HTTPClient.h>
#include <WiFi.h>
#include <Arduino.h>

#define FAILS_ALLOWED 5

Send::Send() {
  API_SERVER = "";
  API_KEY = "";
  FAILED_COUNTER = 0;
}

Send::Send(String newApiServer, String newApiKey) {
  API_SERVER = newApiServer;
  API_KEY = newApiKey;
  FAILED_COUNTER = 0;
}

void Send::setApiServer(String newApiServer) {
  API_SERVER = newApiServer;
}

void Send::setApiKey(String newApiKey) {
  API_KEY = newApiKey;
}

void Send::setApi(String newApiServer, String newApiKey) {
  API_SERVER = newApiServer;
  API_KEY = newApiKey;
  FAILED_COUNTER = 0;
}

String Send::json(float humidity, float tempeture) {
  String data = "{\"humidity\": ${H},\"tempeture\": ${T},\"apiKey\": \"${API_KEY}\"}";
  data.replace("${H}", String(humidity));
  data.replace("${T}", String(tempeture));
  data.replace("${API_KEY}", API_KEY);
  return data;
}

int Send::data(float humidity, float tempeture) {
  if (WiFi.status() != WL_CONNECTED) return 0;
  if (API_SERVER == "" || API_KEY == "") return 2;
  
  HTTPClient http;
  http.begin(API_SERVER);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(json(humidity, tempeture));
  http.end();
      
  if (httpResponseCode >= 200 && httpResponseCode <= 299) {
    Serial.println("[SENDER] Data sent succesfully.");
    FAILED_COUNTER = 0;
    return 1;
  } else {
    FAILED_COUNTER++;
    Serial.print("[SENDER] HTTP Error code (");
    Serial.print(FAILED_COUNTER);
    Serial.print("/");
    Serial.print(FAILS_ALLOWED);
    Serial.print("): ");
    Serial.println(httpResponseCode);
  }

  if (FAILED_COUNTER >= FAILS_ALLOWED) {
    Serial.print("[SENDER] Failed to send data ");
    Serial.print(FAILED_COUNTER);
    Serial.println(" times, configuration needed.");
    API_SERVER = "";
    API_KEY = "";
    return 2;
  }
      
  return 0;
}
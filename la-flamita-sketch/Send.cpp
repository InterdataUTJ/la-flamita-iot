#include "Send.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

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

void Send::setApi(String newApiServer, String newApiKey) {
  API_SERVER = newApiServer;
  API_KEY = newApiKey;
  FAILED_COUNTER = 0;
}


int Send::send(JsonDocument document) {
  if (WiFi.status() != WL_CONNECTED) return 0;
  if (API_SERVER == "" || API_KEY == "") return 2;
  
  HTTPClient http;
  http.begin(API_SERVER);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + API_KEY);

  String body = "";
  serializeJson(document, body);
  int httpResponseCode = http.POST(body);
  http.end();
      
  if (httpResponseCode >= 200 && httpResponseCode <= 299) {

    digitalWrite(2, HIGH);
    FAILED_COUNTER = 0;
    delay(100);
    digitalWrite(2, LOW);
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
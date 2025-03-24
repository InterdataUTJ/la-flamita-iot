#include "Send.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

#define FAILS_ALLOWED 5
#define URL_ID_PLACEHOLDER "{{API_ID}}"

Send::Send() {
  API_SERVER = "";
  API_KEY = "";
  API_ID = "";
  FAILED_COUNTER = 0;
}

Send::Send(String newApiServer, String newApiKey, String newApiId) {
  API_SERVER = newApiServer;
  API_KEY = newApiKey;
  API_ID = newApiId;
  FAILED_COUNTER = 0;
}

void Send::setApi(String newApiServer, String newApiKey, String newApiId) {
  API_SERVER = newApiServer;
  API_KEY = newApiKey;
  API_ID = newApiId;
  FAILED_COUNTER = 0;
}


int Send::send(JsonDocument document) {
  if (WiFi.status() != WL_CONNECTED) return 0;
  if (API_SERVER == "" || API_KEY == "") return 2;
  
  String urlToUse = API_SERVER;
  urlToUse.replace(URL_ID_PLACEHOLDER, API_ID);

  HTTPClient http;
  http.begin(urlToUse);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + API_KEY);

  String body = "";
  serializeJson(document, body);
  int httpResponseCode = http.POST(body);
  http.end();
      
  if (httpResponseCode >= 200 && httpResponseCode <= 299) {

    digitalWrite(2, HIGH);
    FAILED_COUNTER = 0;
    delay(50);
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
    Serial.print("[SENDER] Failed to ask or send data ");
    Serial.print(FAILED_COUNTER);
    Serial.println(" times, configuration needed.");
    API_SERVER = "";
    API_ID = "";
    API_KEY = "";
    return 2;
  }
      
  return 0;
}


JsonDocument Send::ask() {

  JsonDocument response;
  if (WiFi.status() != WL_CONNECTED) return response;
  if (API_SERVER == "" || API_KEY == "") {
    response["INTERNAL_ARDUINO_SHOULD_CONFIG"] = true;
    return response;
  }
  
  String urlToUse = API_SERVER;
  urlToUse.replace(URL_ID_PLACEHOLDER, API_ID);

  HTTPClient http;
  http.begin(urlToUse);
  http.addHeader("Authorization", "Bearer " + API_KEY);
  int httpResponseCode = http.GET();
      
  if (httpResponseCode >= 200 && httpResponseCode <= 299) {

    FAILED_COUNTER = 0;
    digitalWrite(2, HIGH);
    delay(50);
    digitalWrite(2, LOW);
    delay(100);
    digitalWrite(2, HIGH);
    delay(50);
    digitalWrite(2, LOW);
    
    String payload = http.getString();
    deserializeJson(response, payload);
    http.end();
    return response;

  } else {

    FAILED_COUNTER++;
    Serial.print("[SENDER] HTTP Error code (");
    Serial.print(FAILED_COUNTER);
    Serial.print("/");
    Serial.print(FAILS_ALLOWED);
    Serial.print("): ");
    Serial.println(httpResponseCode);
    http.end();

  }

  
  if (FAILED_COUNTER >= FAILS_ALLOWED) {
    Serial.print("[SENDER] Failed to ask or send data ");
    Serial.print(FAILED_COUNTER);
    Serial.println(" times, configuration needed.");
    API_SERVER = "";
    API_KEY = "";
    API_ID = "";
    response["INTERNAL_ARDUINO_SHOULD_CONFIG"] = true;
    return response;
  }
      
  return response;
}
#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
String serverName = SECRET_SERVER;
const char* ssid = SECRET_SSID;
const char* password = SECRET_PSW;
unsigned long lastTime = 0;
unsigned long timerDelay = 15000;


void setup() {
  Serial.begin(115200); 
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.println("Connecting");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("Connected to WiFi.");
}



void loop() {
  if (WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    String serverPath = serverName + "?h=" + dht.readHumidity() + "&t=" + dht.readTemperature();
    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET();
      
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
      
    // Free resources
    http.end();
    delay(timerDelay);
  } else {
    Serial.println("WiFi Disconnected");
    delay(5000);
  }
}
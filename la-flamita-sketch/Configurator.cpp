#include "ArduinoJson/Variant/JsonVariant.hpp"
#include "WiFiType.h"
#include "Configurator.h"
#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>

#define SERIAL_BAUDRATE 115200
#define WIFI_TIMEOUT 10000
#define PREFERENCES_PARTITION "la-flamita-iot"

String WIFI_STRING_STATUS[7] = { "CONNECTING", "NO_SSID", "SCAN_COMPLETED", "CONNECTED", "FAILED", "LOST", "DISCONNECTED" };


Configurator::Configurator() {
  needConfig = true;
  Preferences pref;
  API_KEY = "";
  API_URL = "";
  WIFI_SSID = "";
  WIFI_PASS = "";
}

bool Configurator::begin() {
  // Setup mode
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println("[CONFIGURATOR] Starting system.");

  pref.begin(PREFERENCES_PARTITION);
  WIFI_SSID = pref.getString("WIFI_SSID", "");
  WIFI_PASS = pref.getString("WIFI_PASS", "");
  API_KEY = pref.getString("API_KEY", "");
  API_URL = pref.getString("API_URL", "");

  return checkConfig();
}


bool Configurator::checkConfig() {

  Serial.println("[CONFIGURATOR] Trying to connect to WiFi.");
  if (WIFI_PASS == "" || WIFI_SSID == "") {

    Serial.println("[CONFIGURATOR] Configuration error. No SSID or WiFi Password.");
    return false;

  }

  unsigned long time = millis();
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while(WiFi.status() != WL_CONNECTED) {
    if (millis() >= (time + WIFI_TIMEOUT)) {
      Serial.println("[CONFIGURATOR] Timeout, couldnt connect to WiFi.");
      return false;
    }
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("[CONFIGURATOR] WiFi connected.");
    needConfig = false;
  } else Serial.println("[CONFIGURATOR] Could not connect to WiFi.");

  return (WiFi.status() == WL_CONNECTED);
}


// Devuelve TRUE si se actualizo algun valor
bool Configurator::step() {
  while (Serial.available()) {
    String command = Serial.readString();
    String data = "";

    // Eliminar saltos de linea
    command.replace("\n", "");
    command.replace("\r", "");

    // Buscar si es un comando de set
    int index = command.indexOf(' ');
    if (index != -1) {
      data = command.substring(index + 1);
      command = command.substring(0, index);
    }


    if (command == "LF_STATUS") {

      Serial.println();
      Serial.println("=============================");
      Serial.println("==  La Flamita IoT Status  ==");
      Serial.println("=============================");
      Serial.println();

      Serial.print  ("SENSOR_STATE:  ");
      if (needConfig) Serial.println("STOPPED");
      else Serial.println("WORKING");

      Serial.print  ("WIFI_STATUS:   ");
      Serial.println(WIFI_STRING_STATUS[WiFi.status()]);
      if (WiFi.status() == WL_CONNECTED) {
        Serial.print(" - LOCAL_IP:   ");
        Serial.println(WiFi.localIP());
        Serial.print(" - GATEWAY_IP: ");
        Serial.println(WiFi.gatewayIP());
      }

      Serial.println("WIFI_SSID:     " + WIFI_SSID);
      Serial.println("WIFI_PASS:     " + WIFI_PASS);
      Serial.println("API_URL:       " + API_URL);
      Serial.println("API_KEY:       " + API_KEY);
      Serial.println();
      prompt();
      return false;
    
    } else if (command == "LF_HELP") {

      Serial.println();
      Serial.println("===========================");
      Serial.println("==  La Flamita IoT Help  ==");
      Serial.println("===========================");
      Serial.println();
      Serial.println("LF_HELP:                        Muestra los comandos disponibles. ");
      Serial.println("LF_STATUS:                      Muestra el estado y configuracion del sensor.");
      Serial.println("LF_CLEAR:                       Elimina toda la información almacenada.");
      Serial.println("LF_STOP:                        Detiene el envio de información.");
      Serial.println("LF_START:                       Arranca el sensor intenta conectarse a WiFi.");
      Serial.println("LF_SET_WIFI_SSID [SSID]:        Configura el SSID WiFi.");
      Serial.println("LF_SET_WIFI_PASS [PASS]:        Configura la contraseña WiFi.");
      Serial.println("LF_SET_API_ID [DEVICE] [ID]:    Configura el token de la API.");
      Serial.println("LF_SET_API_KEY [DEVICE] [KEY]:  Configura el token de la API.");
      Serial.println("LF_SET_API_URL [URL]:           Configura el URL para enviar datos a la API.");
      Serial.println();
      prompt();
      return false;

    } else if (command == "LF_CLEAR") {

      Serial.println();
      Serial.println("[Configurator] Storage ereased.");
      API_KEY = "";
      API_URL = "";
      WIFI_SSID = "";
      WIFI_PASS = "";
      pref.clear();
      Serial.println();
      prompt();
      return false;

    } else if (command == "LF_STOP") {

      Serial.println();
      Serial.println("[CONFIGURATOR] Sensor stopped.");
      Serial.println();
      digitalWrite(2, HIGH);
      needConfig = true;
      prompt();
      return false;

    }  else if (command == "LF_START") {

      Serial.println();
      if (checkConfig()) {
        digitalWrite(2, LOW);
        needConfig = false;
      } else needConfig = true;
      Serial.println();
      prompt();
      return true;

    } else if (command == "LF_SET_WIFI_SSID") {

      Serial.println();
      WIFI_SSID = data;
      pref.putString("WIFI_SSID", WIFI_SSID);
      Serial.println("[CONFIGURATOR] WIFI_SSID updated: " + WIFI_SSID);
      Serial.println();
      prompt();
      return false;

    } else if (command == "LF_SET_WIFI_PASS") {

      Serial.println();
      WIFI_PASS = data;
      pref.putString("WIFI_PASS", WIFI_PASS);
      Serial.println("[CONFIGURATOR] WIFI_PASS updated: " + WIFI_PASS);
      Serial.println();
      prompt();
      return false;

    } else if (command == "LF_SET_API_KEY") {

      int indexData = data.indexOf(' ');
      String device = "";
      String key = "";

      Serial.println();
      if (indexData == -1) {
        Serial.println("[CONFIGURATOR] Debes introducir ambos parametros.");
        Serial.println();
        prompt();
        return false;
      }

      key = data.substring(indexData + 1);
      device = data.substring(0, indexData);

      API_KEY = data;
      pref.putString("API_KEY", API_KEY);
      Serial.println("[CONFIGURATOR] API_KEY updated: " + API_KEY);
      Serial.println();
      prompt();
      return true;

    } else if (command == "LF_SET_API_URL") {

      Serial.println();
      API_URL = data;
      pref.putString("API_URL", API_URL);
      Serial.println("[CONFIGURATOR] API_URL updated: " + API_URL);
      Serial.println();
      prompt();
      return true;

    } else if (command == "") {

      prompt();
      return false;

    }


    Serial.println();
    Serial.println("Comando no reconocido. Prueba 'LF_HELP' para ver los comandos disponibles.");
    Serial.println();
    prompt();
    return false;
  }

  return false;
}

void Configurator::prompt() {
  Serial.print("\033[32mlaflamita\033[0m:\033[34m~\033[0m$ ");
}

void Configurator::askConfig() {
  needConfig = true;
  digitalWrite(2, HIGH);
  Serial.println("[CONFIGURATOR] New configuration needed.\n");
  prompt();
}

bool Configurator::canSend() {
  if (WIFI_SSID == "") return false;
  if (WIFI_PASS == "") return false;
  if (API_KEY == "") return false;
  if (API_URL == "") return false;
  if (WiFi.status() != WL_CONNECTED) return false;
  return !needConfig;
}

// Getters
String Configurator::getApiUrl() { return API_URL; }

String Configurator::getApiId(String device) { 
  if (!API_DEVICES[device].is<JsonVariant>()) return "";
  return API_DEVICES[device][0]; 
}

String Configurator::getApiKey(String device) { 
  if (!API_DEVICES[device].is<JsonVariant>()) return "";
  return API_DEVICES[device][1];
}
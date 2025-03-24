#include <Arduino.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <TickTwo.h>
#include "Send.h"
#include "Configurator.h"
#include "BLE.h"

// BLE
BLE bleServer;


// Led config
#define LED_PIN 13
#define LED_CONFIG_SLOT 2
bool ledEstado = false;
Send ledSender;


// buzzer config
#define BUZZER_PIN 12
#define BUZZER_INTERVALO 500
#define BUZZER_TONE_1 2000
#define BUZZER_TONE_2 2500
#define BUZZER_CONFIG_SLOT 1
Send buzzerSender;
bool estadoBuzzer = false;
bool tonoActual = false;


// Sensor CONFIG
#define DHT_PIN 14
#define DHT_TYPE DHT11
#define DHT_CONFIG_SLOT 0
#define DHT_TEMP_LIMIT 35
DHT dht(DHT_PIN, DHT_TYPE);
Send dht11Sender;


// Internal config
#define SENDER_DELAY 5000
#define ASK_DELAY 2500
Configurator config;
unsigned long nextSend = 0;
unsigned long nextAsk = 0;


// TImers
void senderTimerCallback();
TickTwo senderTimer(senderTimerCallback, SENDER_DELAY);
void askTimerCallback();
TickTwo askTimer(askTimerCallback, ASK_DELAY);
void buzzerToneStep();
TickTwo buzzerTimer(buzzerToneStep, BUZZER_INTERVALO);




// Get the info to send
JsonDocument getData() {
  JsonDocument root;
  JsonDocument dato;
  
  dato["Temperatura"] = dht.readTemperature();
  dato["Humedad"] = dht.readHumidity();

  root["dato"] = dato;
  return root;
}




void buzzerToneStep() {
  if (!estadoBuzzer) return noTone(BUZZER_PIN);
  tonoActual = !tonoActual;
  tone(BUZZER_PIN, tonoActual ? BUZZER_TONE_1 : BUZZER_TONE_2);
}


void askTimerCallback() {
  if (config.canSend(BUZZER_CONFIG_SLOT)) {
    JsonDocument res = buzzerSender.ask();
    if (res["INTERNAL_ARDUINO_SHOULD_CONFIG"]) config.askConfig();
    if (res["dato"]) estadoBuzzer = (res["dato"] == "1");
  }
  
  if (config.canSend(LED_CONFIG_SLOT)) {
    JsonDocument res = ledSender.ask();
    if (res["INTERNAL_ARDUINO_SHOULD_CONFIG"]) config.askConfig();
    if (res["dato"]) ledEstado = (res["dato"] == "1");
    digitalWrite(LED_PIN, ledEstado ? HIGH : LOW);
  }
}


void senderTimerCallback() {
  if (config.canSend(DHT_CONFIG_SLOT)) {
    JsonDocument data = getData();

    if (data["dato"]["Temperatura"]) {
      if (data["dato"]["Temperatura"] >= DHT_TEMP_LIMIT) {
        bleCallback(); // Iniciar alarma
      }
    }

    int state = dht11Sender.send(data);
    if (state == 2) config.askConfig();
  }
}

void bleCallback() {
  Serial.println("[BLE] Alarma activada");
  estadoBuzzer = true;
  digitalWrite(LED_PIN, HIGH);

  // Enviar estado al servidor
  JsonDocument estadoEncendido;
  estadoEncendido["dato"] = "1";
  if (config.canSend(LED_CONFIG_SLOT)) {
    ledSender.send(estadoEncendido);
  }

  if (config.canSend(BUZZER_CONFIG_SLOT)) {
    buzzerSender.send(estadoEncendido);
  }
}




void setup() {
  // Sensor config
  dht.begin();

  // Internal config
  pinMode(2, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  config.begin();

  buzzerSender.setApi(config.getApiUrl(), config.getApiKey(BUZZER_CONFIG_SLOT), config.getApiId(BUZZER_CONFIG_SLOT));
  dht11Sender.setApi(config.getApiUrl(), config.getApiKey(DHT_CONFIG_SLOT), config.getApiId(DHT_CONFIG_SLOT));
  ledSender.setApi(config.getApiUrl(), config.getApiKey(LED_CONFIG_SLOT), config.getApiId(LED_CONFIG_SLOT));

  // Start timers
  senderTimer.start();
  askTimer.start();
  buzzerTimer.start();

  bleServer.server("ESP32-DHT11-BLE-SERVER", bleCallback);
}


void loop() {
  // Setup new API details if configuration changed
  if (config.step()) {
    buzzerSender.setApi(config.getApiUrl(), config.getApiKey(BUZZER_CONFIG_SLOT), config.getApiId(BUZZER_CONFIG_SLOT));
    dht11Sender.setApi(config.getApiUrl(), config.getApiKey(DHT_CONFIG_SLOT), config.getApiId(DHT_CONFIG_SLOT));
    ledSender.setApi(config.getApiUrl(), config.getApiKey(LED_CONFIG_SLOT), config.getApiId(LED_CONFIG_SLOT));
  }


  // Update timers
  senderTimer.update();
  askTimer.update();
  buzzerTimer.update();
}
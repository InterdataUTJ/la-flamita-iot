<h1 align="center">
  <img src="./la-flamita-iot.svg" alt="la-flamita-iot" width="200">
  <br>
  La Flamita IoT
  <br>
  <br>
</h1>

<p align="center">
  <a href="https://www.arduino.cc/"><img src="https://img.shields.io/badge/Built_using-Arduino-lightgray.svg?logo=arduino" alt="js-semistandard-style"></a>
  <a href="https://www.espressif.com/en/products/socs/esp32"><img src="https://img.shields.io/badge/Made_for-ESP32-orangered.svg?logo=espressif" alt="js-semistandard-style"></a>
</p>

> [!NOTE]
> Este es un `submodule` que forma parte del proyecto [`la-flamita`](https://github.com/InterdataUTJ/la-flamita/).

Desarrollo de IoT para taquería la flamita. La solución se enfoca en aplicar sensores de temperatura modulares que se integren fácilmente con la `API` desarrollada en [`la-flamita-admin`](https://github.com/InterdataUTJ/la-flamita-admin/) para poder monitorearse.

# Documentación 📕

Este proyecto esta formado por dos placas ESP32 interconectadas usando el protocolo `BLE` y a su vez conectadas a la `API` de `la-flamita-admin` mediante `WiFi`.


## `ESP32` - `DHT11`

Este modelo incluyte un sensor de temperatura y humedad, junto a una alarma formada por un buzzer y un led. La alarma puede activarse al sobrepasar un limite de lecturas o al recibir una solicitud de un cliente `bluetooth`.

Este tipo de placa tiene cargado el codigo [`la-flamita-dht11`](./la-flamita-dht11/), su esquema de conección es:

### Sensor DHT11 🌡️

| **DHT11** | **ESP32**  |
|-----------|------------|
| Vcc       | 3v3        |
| GND       | GND        |
| DATA      | GPIO14     |

### Led 💡

Es necesario agregar una resistencia de entre 100Ω y 200Ω entre los pines negativos.

| **Led**     | **ESP32** |
|-------------|-----------|
| GND         | GND       |
| Vcc PIN     | GPIO13    |


### Buzzer pasivo 🎵

| **Buzzer**  | **ESP32** |
|-------------|-----------|
| GND         | GND       |
| Vcc PIN     | GPIO12    |



## `ESP32` - `Sensor Luz`

Este modelo incluye unicamente un sensor de luz que envia cada cierto tiempo su estado a la API. Además, al estar activo durante alrededor de 30 segundos (es decir, tener la puerta del refrigerador abierta) envia una solicitud por bluetooth al otro modelo para activar la alarma.

Este tipo de placa tiene cargado el codigo [`la-flamita-luz`](./la-flamita-luz//), su esquema de conección es:

### Sensor de Luz 💡

| **DHT11** | **ESP32** |
|-----------|-----------|
| Vcc       | 3v3       |
| GND       | GND       |
| DATA      | GPIO4     |

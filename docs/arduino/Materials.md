# Materiales 🧩

El módulo puede funcionar con baterías o sin baterías, estas solo se conectan a los pines Vin de entrada del ESP32, por lo que no afecta para nada al funcionamiento, comprueba la guía de conexiones para más información.

> [!WARNING]  
> El firmware usa la biblioteca [`Preferences`](https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/preferences.html) de `ESP32`, por lo que podria no funcionar en otra placa de desarrollo.

Los componentes básicos del módulo son los siguientes:

1. `ESP32`. (_Probado en un `ESP32-WROOM-32`_).
2. Sensor `DHT11`.
3. Led RGB de `cátodo común`.
4. 3x resistencias de `290 Ω`. (_o un valor aproximado_).

Además, en caso de querer alimentar el dispositivo con bacterias y no por USB, se necesitarán:

4. 3xAA de 1.5v. (_El diagrama muestra 4 porque [`fritzing`](https://github.com/fritzing/fritzing-app/tree/1.0.3) no cuenta con módulos de 3 baterías_).
5. Switch toggle de 3 pines.
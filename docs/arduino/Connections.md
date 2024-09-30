# Conexiones 🔌

A pesar de que el diagrama muestra unos pines, es muy probable que tu placa `ESP32` o incluso los sensores tengan un `PINOUT` diferente, por lo que es importante verificar la documentación técnica y corroborar la siguiente información.




## Sensor DHT11 🌡️

Es fundamental tomar en cuenta que estoy usando un sensor DHT11 de 3 pines con resistencia, en caso de utilizar uno sin resistencia o de 4 pines deberás usar una resistencia [`pull-up`](https://startingelectronics.com/beginners/arduino-tutorial-for-beginners/arduino-DHT11-sensor-tutorial/) en el pin de datos.

| **DHT11** | **ESP32** |
|-----------|-----------|
| Vcc       | 3v3       |
| GND       | GND       |
| DATA      | GPIO4     |




## Led RGB 💡

Es importante tomar en cuenta que estamos usando un Led RGB de [cátodo común](https://micrologt.blogspot.com/2017/03/como-diferenciar-un-rgb-de-catodo-comun.html), es decir, solo tiene un PIN negativo y 3 positivos (para cada color).

Además, deberás agregar una de las resistencias entre cada PIN positivo y el ESP32.

| **Led RGB** | **ESP32** |
|-------------|-----------|
| GND         | GND       |
| Red PIN     | GPIO18    |
| Green PIN   | GPIO19    |
| Blue PIN    | GPIO21    |




# Baterías 🔋

Esta parte es totalmente opcional, pues el módulo puede funcionar sin esto alimentándolo por el puerto USB con una powerbank o cualquier otro medio, siempre y cuando entregue un mínimo de `5v`.

En el caso de querer utilizar baterías, te recomiendo un mínimo de 3xAA de `1.5v` para poder sobrepasar los `3.3v` sin problemas y asegurar el buen funcionamiento de la placa. Puedes usar otras baterías siempre y cuando la placa no reciba más de `5v`, por lo que esta es mi recomendación.

Recomiendo agregar un toggle switch entre el pin `Vout` de la batería `Vin` de la placa para poder apagarla. 

| **Baterías** | **ESP32** |
|--------------|-----------|
| GND          | GND       |
| Vout         | Vin       |
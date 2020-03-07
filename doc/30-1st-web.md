

![Austral Ingenieria](https://encrypted-tbn0.gstatic.com/images?q=tbn%3AANd9GcQooGo7vQn4t9-6Bt46qZF-UY4_QFpYOeh7kVWzwpr_lbLr5wka)


# Proyecto 30-1st-web 

## Secciones

- [Objetivo](#objetivo)
- [Programa](#programa)
- [platformio.ini](#platformio.ini)
- [Constantes](#constantes)


## Objetivo

>   El objetivo es construir una página muy simple de Web que reside en el SoC (ya sea _ESP32_ o _ESP8266_).
>   El único objeto de esta página es poner un texto de saludo acorde al SoC donde corra.
>   Usar con cualquier placa que incluya a los procesadores indicados

### Programa

Para poder crear una página de Web, aparte de la biblioteca residente que permite trabajar con WiFi, se debe tener también una biblioteca que permita crear un servidor de Web.

De la misma manera que en el caso de la biblioteca de WiFi, el archivo de encabezamiento (_header file_) que debe usarse es distinto tanto para _ESP32_ como _ESP8266_; por lo tanto, se seleccionarán utilizando la característica de compilación condicional basado en la existencia o no del símbolo ___ESP32___.

Para ello, debería incluirse un trozo de código que pueda seleccionar, entonces, cuales son los archivos de encabezamiento que se usarán; una posibilidad de dicho código se muestra aquí:

~~~ C++
#ifdef ESP32

#include <WiFi.h>
#include <WebServer.h>

#else

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#endif
~~~

Si se obervan las líneas entre 9 y 24, se ve que parte de ese código est'entre ellas; sin embargo, como hay más cosas que dependen del tipeo de placa conectada, se encuentan más definiciones en ese trozo de código.

Una de ellas es que el mensaje que aparecerá una vez direccionada la página de Web desde un _browser_ también depende de la placa conectda.

Como la página de Web se construirá disponiendo de la creación de un objeto tipo _Servidor_ y el tipo de objeto es distinto, de acuerdo se trate de _ESP32_ o de _ESP8266_, entonces también se lo hce a través de un símbolo intermedio que se llama _ServerObject_

En definitiva, el código de selección queda como sigue:

~~~ C++
#ifdef ESP32

#include <WiFi.h>
#include <WebServer.h>

#define ServerObject    WebServer
#define HelloMsg        "Hello from ESP32 !"

#else

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define ServerObject    ESP8266WebServer
#define HelloMsg        "Hello from ESP8266 !"
#endif

~~~
### platformio.ini

#### Constantes

Como se habrá observado, hay cinco constantes que se definen en el archivo _platformio.ini_ para mayor comodidad de mantenimiento; de esa manera, no es necesario entrar en los archivos de código para dicho mantenimiento.

Si bien el proyecto está armado para poder compilarse para dos procesadores (_ESP8266_ y _ESP32_), cuatro de estas constantes son comunes a ambos, por lo cual están definidas en _build_flags_ de la sección _[env]_; dichas definiciones están contenidas entre las líneas 28 y 22 y se copian más abajo

~~~ ini
build_flags =
     -DTIME_ZONE=-3
     -DMY_SSID=\"EDUMEL51\"
     -DMY_PASS=\"0044415146\"
     -DNTP_SERVER=\"ar.pool.ntp.org\"
~~~

Obsérvese que, por definirse en tres de ellas constantes _string_, debe escaparse el caracter ___"___ mediante el caracter barra invertida.

La otra constante que falta es _SERIAL\_BAUD_, lo cual lleva una disgresión: en este caso, lo importante es que esta constante, que se usa para inicializar por programa la interfase serie, esté en un todo de acuerdo con la definición de _hardware_ de la interfase que se entrega por _monitor\_speed_ en este archivo.

Ello se logra como se muestra en la línea 29 para la inicialización de _hardware_ mediante:

~~~ ini
monitor_speed = 115200
~~~

Bastaría, por lo tanto, colocar bajo _build\_flags_:

~~~ ini
build_flags =
    -DSERIAL_BAUD=115200
~~~

pero eso significaría que se debería estar atento a que los números que se colocasen en las líneas 29 y 32 _fuesen exactamente iguales_.

Sería, entonces, mejor vincular automáticamente ambos lo cual se logra en la línea 32 recabando el valor de el ítem _monitor\_speed_ de la misma sección _env_; en definitiva, podemos aquí mostrar el _env_ correspondiente a _ESP32_ que se encuentra entre las líneas 26 y 32

~~~ ini
[env:wemos_d1_mini32]
platform = espressif32
board = wemos_d1_mini32
monitor_speed = 115200
build_flags =
    ${env.build_flags}
    -DSERIAL_BAUD=${env:wemos_d1_mini32.monitor_speed}
~~~

De esta forma, se pueden poner distintos _baud\_rate_ para _ESP32_ y _ESP8266_, que es justamente lo que se ha hecho en este caso, ya que para este último dicho _baud\_rate_ es de __74880__

Claro, la pregunta que queda es ¿porqué se ha elegido tal _baud\_rate_ no normalizado para este caso? La respuesta es que, no sabemos porqué, cuando _nace_ después de un _reset_ este procesador, amanece con dicho _baud\_rate_; de esta manera, con este poco común _baud\_rate_ se podrán observar todos los caracteres que, a partir de _reset_, se envían a la interfase serie.




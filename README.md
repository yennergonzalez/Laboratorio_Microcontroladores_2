# Laboratorio Microcontroladores 2: GPIOs, Timers y FSM

En este laboratorio se implementó un sistema de cruce peatonal simplificado que consiste de un semáforo vehicular y dos semáforos peatonales.
Para este, las luces de cada semáforo debían operar de acuerdo a una secuencia definida y bajo un tiempo establecido si en el estado de reposo se presionaba
alguno de los botones de los semáforos peatonales.

La implementación realizada hace uso de interrupciones para definir las temporizaciones y detectar el presionado de los botones.

## Autor
Yenner Josué González Araya - B83375

## Circuito realizado
El circuito es manejado por un microcontrolador ATtiny4313 y corresponde al esquemático a continuación:
![alttext](https://github.com/yennergonzalez/Laboratorio_Microcontroladores_2/blob/main/Figuras_ReadMe/Captura_Circuito_Con_Medidores.png)


## Diagrama de temporización obtenido
La secuencia de temporización obtenida para las señales corresponde a la propuesta en el enunciado y se muestra en la figura a continuación:
![alttext](https://github.com/yennergonzalez/Laboratorio_Microcontroladores_2/blob/main/Figuras_ReadMe/Captura_Diagramas_Tiempo.png)

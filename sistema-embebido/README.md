# Sistema Embebido
En este subdirectorio se encontrará el código para Arduino para un robot que lleva objetos hacia la persona correspondiente, siguiente la señal de bluetooth emitida por su teléfono.

## Preparación

### Componentes
- 1 x Arduino Mega 2560
- 1 x Módulo Bluetooth HC-05
- 1 x Módulo de Sensor Ultrasónico HC-SR04
- 1 x CI Puente-H L293D
- 1 x Transistor LM7805 (regulador de voltaje con salida de 5v)
- 2 x Motor DC 6v
- 2 x Rueda para motor
- 1 x Rueda loca
- 1 x Placa de presión por peso (hecha por nosotros)
- 1 x LED rojo
- 1 x Resistencia 220 ohms
- 1 x Pila 9v
- 1 x Conector para pila 9v
- 1 x Parlante
- Cables

## Sensores
Los sensores del robot detectan cuando el robot tiene un objeto en su caja, para empezar el recorrido. Tiene un sensor bluetooth a través del cuál detecta la señal de los dispositivos cercanos. Esta señal será utilizada para seguir a la persona correspondiente. Y por último, tiene un sensor de ultrasonido, con el cual detectará los obstáculos que se encuentre en su camino.

## Actuadores
El robot cuenta de dos motores que actuan en base a lo medido por los sensores y que permitirán que el robot alcance su objetivo. También tiene actuadores (parlante y LED) que permitirán informar cuando llegó a su destino.

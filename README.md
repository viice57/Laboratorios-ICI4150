# Laboratorio 2 ICI4150

Esta rama del repositorio contiene el Laboratorio 2, la cual ya incorpora la totalidad de los sensores, actuadores y controladores entregados para el desarrollo de un robot móvil de 3 ruedas (2 de tracción, 1 de dirección). El objetivo fue implementar un único algoritmo que permita al robot desplazarse en un entorno controlado, usando los datos del mismo entorno para tomar decisiones autónomas. Más abajo se encuentra el enunciado con el desglose de las actividades realizadas.
 
## Integrantes

- Cristofer Contreras
- Ignacio Cuevas
- Vicente Morales

## Contenidos

Para este laboratorio, se incluye lo siguiente dentro de las secciones y como anexo cuando corresponda:

- Respuestas a las preguntas (parte 1 y 2)
- Sketches
- Esquema de conexión
- Videos del funcionamiento

## Anexos

- Enlace al repositorio: https://github.com/viice57/Laboratorios-ICI4150.git
- Descarga Arduino IDE: https://www.arduino.cc/en/software/
- Descarga Fritzing: https://github.com/fritzing/fritzing-app/releases/
- Esquema actual (act. 04/06/25): ![Esquema](https://github.com/viice57/Laboratorios-ICI4150/blob/Laboratorio-2/Imágenes/Esquema.png)
- Imagen del robot: ![Frente](https://github.com/viice57/Laboratorios-ICI4150/blob/Laboratorio-2/Imágenes/Frente.jpg)

---

# Desarrollo Laboratorio 2

## Tema: Sensores, Percepción y Planificación Básica con procesamiento de datos en Robótica Móvil.

## Objetivos
    * Sensores y Percepción: Aprender a procesar los datos de los sensores (ultrasónico e IMU) mediante técnicas de filtrado y umbralización para mejorar la precisión de la percepción.
    * Filtrado de señales: Aplicar técnicas básicas de filtrado para reducir el ruido en los datos obtenidos por los sensores.
    * Umbralización: Definir umbrales adecuados para detectar obstáculos de manera más confiable.
    * Planificación Básica: Realizar un control de navegación basado en la percepción filtrada y umbralizada para evitar obstáculos y en el color detectado.

## Materiales
    * Arduino UNO
    * Chasis de robot móvil con dos motores DC, basado en impresión 3D.
    * Sensores (Ultrasónicos, y RGB) para detección de obstáculos y color.
    * Sensor IMU (MPU 6050) para medición de inclinación y giro.
    * Servomotor SG90.
    * Batería y módulo de alimentación.
    * Driver de motores (puente H) L298N.
    * Protoboard y cables de conexión.

## Herramientas utilizadas
    * Arduino IDE: Para crear sketches y ejecutarlos.
    * Fritzing: Para diseñar los esquemas de conexión.

## Parte 1: Configuración del Hardware y pruebas iniciales

Tanto para la parte 1 como para la parte 2, se utilizó el mismo esquema definido en la imagen "Esquema.png". Esto nos permitió analizar mejoras en el cableado y pinout de forma temprana, sin necesidad de tener físicamente al robot para definir cómo conectar y alimentar cada componente.

A grandes rasgos, no fue difícil incorporar el sensor HC-SR04 (Ultrasónico), dado que su uso es bastante intuitivo y no requiere mayor corrección; aunque, en el caso del sensor TCS34725 (Detector de luz RGB) sí tuvimos algunas dificultades al momento de definir los umbrales para detectar color, más no al conectarlo con el resto de componentes.

Para finalizar, dentro de las mejoras que se realizaron están: La disposición de cada componente fue mejorada con respecto al Laboratorio 1 para mejorar la estética y para poder dar espacio a otros sensores y actuadores. También se decidió usar dos baterías 18650 en serie como única fuente de alimentación para todo el robot (7.4V nominal), en respuesta a los problemas previos con la batería PP3 (9V nominal).

### Criterios de evaluación
* Conectar los sensores ultrasónico HC-SR04 y TCS34725 en Arduino.
* Programar Arduino para leer la distancia con HC-SR04 y mostrarla en el monitor serie.
* Programar Arduino para leer los valores RGB y mostrar el color detectado.
* Analizar la precisión de los sensores en diferentes condiciones (luz, superficie, distancia).

### Preguntas Parte 1

1. **¿Qué es la percepción en robótica y por qué es fundamental en sistemas autónomos?**

    **R:** La percepción es la capacidad del robot para recolectar datos del mundo exterior (y a veces de su propio estado interno) y transformarlos en información útil para navegar, interactuar o cumplir tareas. Gracias a la percepción, un robot puede construir una representación de su entorno que le permita tomar decisiones inteligentes y actuar de forma autónoma.

    ---

2. **En el sensor ultrasónico HC-SR04, ¿qué parámetro se mide para calcular la distancia?**

    **R:** El HC-SR04 mide el tiempo de viaje del pulso ultrasónico (duración entre el envío en TRIG y la recepción del eco en ECHO). Con ese tiempo 𝑡 y conociendo la velocidad del sonido 𝑣 ≈ 343 m/s, se calcula la distancia con la expresión 𝑣𝑡/2.

    ---

3. **¿Cómo influye el ruido en las mediciones del sensor ultrasónico y cómo podría reducirse?**

    **R:** El ruido puede influir en las lecturas, que pueden variar aleatoriamente (ruido eléctrico, interferencias, variaciones de temperatura o humedad), causando mediciones inconsistentes o picos falsos.

    Para reducir estas variaciones, normalmente se aplican las siguientes estrategias:
    * Promediar múltiples lecturas (media móvil) para suavizar la señal.
    * Aplicar filtros pasa-bajos o media ponderada para atenuar variaciones de alta frecuencia.
    * Calibrar el sensor (offset y escala) usando mediciones en condiciones controladas. Estas técnicas mejoran la precisión y confiabilidad de la distancia medida.

## Parte 2: Procesamiento de datos y Algoritmo de Navegación

En la parte 2 de este laboratorio, para los obstáculos se definieron los siguientes colores:

* **Rojo:** Indica detención total. El robot no podrá moverse sin que una persona lo indique.
* **Verde:** El robot puede continuar navegando, aunque debe ejecutar un cambio de luz con 2 parpadeos.
* **Azul:** Indica que el robot puede continuar, pero debe cambiar su dirección en múltiplos de 90°.

En cuanto a dificultades, lo más difícil y mejorable fue la navegación entre obstáculos, incluyendo la interacción con colores como el azul. Ocupando el IMU se pierde algo de precisión y es difícil hacer que el robot navegue sin impactar algún elemento del entorno.

### Criterios de evaluación
* Aplicar umbralización al sensor ultrasónico para detectar si hay un
obstáculo a menos de 10cm.
* Definir umbrales para detectar colores, rojo, verde y azul usando el
sensor RGB.
* Implementar un algoritmo en Arduino que detenga el robot ante obstáculos y cambie de dirección según el color detectado.
* Probar navegación en un circuito con obstáculos y superficies en diferentes colores.
* Ajustar parámetros para mejorar la detección y estabilidad del sistema.
* Implementación de estrategias de navegación basadas en reglas.

### Preguntas Parte 2

1. **Si el robot detecta el color rojo en el suelo, ¿qué acción debería tomar?¿Por qué?**

    **R:** Ya que el sensor TCS34725 está al frente, el robot debe detenerse inmediatamente si detecta un objeto o pared que coincida con el color rojo. Esto, basándose en la idea de que el color rojo está comúnmente asociado fallas o interrupciones de la ejecución normal.

    ---

2. **Si el sensor ultrasónico detecta valores erráticos, ¿qué estrategias podrías aplicar para mejorar la precisión?**

    **R:** Lo primero que se haría (y que tuvimos que aplicar), es identificar cuál valor se repite más y medir la distancia real con una regla. Si las lecturas varían demasiado entre sí, sin necesidad de que el robot esté moviéndose, se aplicaría un factor de calibración y luego se promediaría el valor de la distancia leída. Esto garantiza que la precisión es alta y no habrán mediciones erráticas en el entorno cercano.

    ---

3. **Si tuvieras que integrar un nuevo sensor para mejorar la navegación del robot, ¿cuál eligirías y por qué?**

    **R:** Lo más útil y que nos hizo falta, es un sensor de posición (encoder) para cada rueda, con la finalidad de no depender de los datos del IMU para poder desplazarse y conocer la posición. Aunque fue difícil en la entrega anterior, no es imposible, pero no nos funcionó.

    ---

4. **¿Cuál es el tiempo de respuesta del robot al detectar un cambio de color?**

    **R:** Prácticamente instantáneo para nuestra percepción. Lógicamente no es así, pero la mayor demora proviene del sensor RGB como tal, dado que el parámetro de tiempo de escaneo está fijo en 600 ms.

---
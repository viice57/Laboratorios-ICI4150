# Laboratorios ICI4150

Este repositorio contiene los laboratorios prácticos y actividades desarrolladas en el ramo Robótica y Sistemas Autónomos, primer semestre del año 2025.
 
## Integrantes

- Cristofer Contreras
- Ignacio Cuevas
- Vicente Morales

## Contenidos

Para cada módulo, se incluye lo siguiente

* Videos del funcionamiento
* Respuestas a las preguntas (parte 1 y 2)
* Sketches
* Esquema de conexión

## Anexos

* Enlace al repositorio: https://github.com/viice57/Laboratorios-ICI4150.git
* Descarga Arduino IDE: https://www.arduino.cc/en/software/
* Descarga Fritzing: https://github.com/fritzing/fritzing-app/releases/
* Esquema actual (act. 04/06/25): https://github.com/viice57/Laboratorios-ICI4150/blob/Laboratorio-2/Esquema.png

---

## Laboratorio 2

* **Tema:** Ensamblar y programar un robot básico. 

* **Objetivos:**
    * Sensores y Percepción: Aprender a procesar los datos de los sensores (ultrasónico e IMU) mediante técnicas de filtrado y umbralización para mejorar la precisión de la percepción.
    * Filtrado de señales: Aplicar técnicas básicas de filtrado para reducir el ruido en los datos obtenidos por los sensores.
    * Umbralización: Definir umbrales adecuados para detectar obstáculos de manera más confiable.
    * Planificación Básica: Realizar un control de navegación basado en la percepción filtrada y umbralizada para evitar obstáculos y en el color detectado.

* **Materiales:**
    * Arduino UNO
    * Chasis de robot móvil con dos motores DC, basado en impresión 3D.
    * Sensores (ultrasónicos, Infrarrojo y RGB) para detección de obstáculos y color.
    * Sensor IMU (MPU 6050) para medición de inclinación y giro.
    * Batería y módulo de alimentación.
    * Driver de motores (puente H) L298N.
    * Protoboard y cables de conexión.

* **Herramientas:**
    * Arduino IDE
    * Fritzing

### Parte 1: Configuración del Hardware y pruebas iniciales
* Conectar los sensores ultrasónico HC-SR04 y TCS34725 en Arduino.
* Programar Arduino para leer la distancia con HC-SR04 y mostrarla en el monitor serie.
* Programar Arduino para leer los valores RGB y mostrar el color detectado.
* Analizar la precisión de los sensores en diferentes condiciones (luz, superficie, distancia).

#### Preguntas Parte 1

1. **¿Qué es la percepción en robótica y por qué es fundamental en sistemas autónomos?**

    **R:** La percepción es la capacidad del robot para recolectar datos del mundo exterior (y a veces de su propio estado) y transformarlos en información útil para navegar, interactuar o cumplir tareas. Gracias a la percepción, un robot construye una representación de su entorno que le permite tomar decisiones inteligentes y actuar de forma autónoma.

    ---

2. **En el sensor ultrasónico HC-SR04, ¿qué parámetro se mide para calcular la distancia?**

    **R:** El HC-SR04 mide el tiempo de vuelo del pulso ultrasónico (duración entre el envío en TRIG y la recepción del eco en ECHO). Con ese tiempo 𝑡 y conociendo la velocidad del sonido 𝑣≈343 m/s, se calcula la distancia como 𝑣𝑡/2.

    ---

3. **¿Cómo influye el ruido en las mediciones del sensor ultrasónico y cómo podría reducirse?**

    **R:** 
    * Influencia del ruido: Las lecturas pueden fluctuar aleatoriamente (ruido eléctrico, interferencias, variaciones de temperatura o humedad), causando mediciones inconsistentes o picos falsos.

    * Reducción del ruido:
        * Promediar múltiples lecturas (media móvil) para suavizar la señal.
        * Aplicar filtros pasa-bajos o media ponderada para atenuar variaciones de alta frecuencia.
        * Calibrar el sensor (offset y escala) usando mediciones en condiciones controladas. Estas técnicas mejoran la precisión y confiabilidad de la distancia medida.

---
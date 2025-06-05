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

---

## Laboratorio 1

* **Tema:** Ensamblar y programar un robot básico.

* **Objetivos:**
    * Comprender los componentes básicos de un robot móvil autónomo.
    * Implementar un control de movimiento basado en sensores sin usar encoders.
    * Aplicar conceptos de cinemática y dinámica en robots móviles.

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

* **Videos de funcionamiento:** No pudimos usar la batería, por lo que tampoco pudimos dejar moverse al robot de forma independiente. En los videos se aprecia que el controlador está alimentado por un transformador de 12V y Arduino por USB.

    * https://www.youtube.com/shorts/1LMhyDZ6efY - Estimación de la posición 
    * https://youtube.com/shorts/COtxGe7M7BY?si=nvGuJBWiBRo-TiwJ - Variación de velocidad + movimientos (adelante, atrás, izquierda, derecha)

Esta entrega se divide en dos módulos. Adicionalmente, se incluye un **esquema de conexión** de componentes.

### Parte 1: Identificación de componentes y configuración

* Conectar Arduino UNO con el driver de motores **L298N** y programar el movimiento básico de los motores (adelante, atrás, giro) sin controlar la velocidad.
* Verificar el funcionamiento del sensor ultrasónico **HC-SR04** midiendo
distancias.
* Analizar los datos del IMU **MPU6050** para medir inclinación o giros
del robot.

#### Preguntas Parte 1

1. **¿Qué función cumplen los sensores, actuadores y controladores en el robot?**

    **R:** Los **sensores** permiten al robot percibir su entorno y estado interno, es decir, recolectar los datos del mundo exterior y/o interior. Por ejemplo, un sensor ultrasónico es un sensor activo que emite y recibe señales para medir distancias

    Los **actuadores** permiten que el robot realice acciones físicas, transformando energía en movimiento o fuerza. Por ejemplo, un motor DC es un actuador eléctrico.

    Los **controladores** procesan la información proveniente de los sensores y coordinan las decisiones en tiempo real, enviando instrucciones a los actuadores para realizar las acciones correspondientes.

    ---

2. **¿Cómo se puede estimar la velocidad sin encoders?**

    **R:** Para hacerlo, se necesita tener una **IMU** tal como la **MPU-6500**. Con el acelerómetro integrado en esta unidad, podemos integrar la aceleración en el tiempo para obtener una estimación de la velocidad lineal. Sin embargo, es susceptible a acumular errores si no se corrige con otros sensores.

    ---

3. **¿Cómo afecta la falta de encoders a la precisión del movimiento?**

    **R:** Impide conocer con exactitud la posición y velocidad actual del robot, ya que no existe una medición directa del desplazamiento de cada una de las ruedas. Esto afecta específicamente a la **odometría**, que se encarga de parámetros relativos a la trayectoria y orientación en el espacio.

    ---

4. **¿Qué es PWM y cómo ayuda a controlar la velocidad de los motores?**

    **R:** Se trata de una técnica que convierte una señal digital en una señal analógica simulada. En motores DC como los que usamos, permite controlar las revoluciones por minuto de cada motor variando el tiempo que la señal permanece activa, lo que regula la potencia entregada a cada motor.

    ---

5. **¿Cómo afecta el control de velocidad a la precisión de la navegación sin encoders?**

    **R:** Si la velocidad no es constante, para estimar la posición del robot se requiere asumir condiciones ideales o realizar cálculos basados en sensores como IMUs, lo que puede generar fallas en la orientación y en la distancia total recorrida. Se necesita la mayor precisión posible.

---
### Parte 2: Cinemática y Dinámica de Robots Móviles usando un IMU

* Aplicar la ecuación de cinemática diferencial para estimar la posición
del robot usando tiempo y velocidad de motores.
* Hacer que el robot se mueva en línea recta y registrar desviaciones
usando el sensor IMU para detectar la inclinación y giro del robot.
* Usar el sensor IMU MPU6050 para medir la inclinación del robot y
ajustar su dirección en tiempo real, realizando correcciones en el movimiento de acuerdo a su orientación.
* Programar el PWM para controlar la velocidad de los motores y hacer
que el robot se mueva a diferentes velocidades sin IMU, variando el
tiempo de activación de los motores.

#### Preguntas Parte 2

1. **¿Cómo se calcula la velocidad del robot sin encoders usando PWM?**

    **R:** Se mide el tiempo que el robot tarda en recorrer una distancia conocida y se calcula la velocidad en forma experimental. Luego, se relaciona esa velocidad con el valor de PWM utilizado para futuras estimaciones bajo condiciones similares.

    ---

2. **¿Cómo afectan los intervalos de tiempo a la trayectoria y velocidad del robot?**

    **R:** Intervalos muy grandes generan respuestas lentas y desvíos en la trayectoria. Si son muy cortos, se capta demasiado ruido. Es importante encontrar un equilibrio y aplicar filtros que suavicen las mediciones del sensor.

    ---

3. **¿Cuáles son las ventajas y desventajas de usar un IMU para ajustar la dirección en lugar de encoders?**

    **R:** La IMU permite detectar giros e inclinaciones sin depender del movimiento de las ruedas, por lo que no sufre con el deslizamiento. Sin embargo, es más sensible al ruido y acumula errores si no se filtra bien la señal.

    ---

4. **¿Qué efecto tiene la inclinación o el giro en el movimiento del robot, y cómo se corrige con el IMU?**

    **R:** Cualquier inclinación o giro cambia la orientación del robot y puede desviar su rumbo. El IMU detecta estos cambios y permite corregir la dirección ajustando las ruedas, manteniendo así una trayectoria más precisa.

---
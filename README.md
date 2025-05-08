# Proyecto ICI4150
 
## Integrantes

- Cristofer Contreras
- Ignacio Cuevas
- Vicente Morales

## Contenido

1. En **Parte 1**: Se encuentran los sketches de prueba para L298N, MPU-6500, HC-SR04.
2. En **Parte 2**: Se encuentra un único sketch para determinar la posición, crear movimiento y registrar inclinaciones.
2. Esquema de conexión (Fritzing)

## Preguntas Parte 1

### 1. ¿Qué función cumplen los sensores, actuadores y controladores en el robot?
**R:** Los **sensores** permiten al robot percibir su entorno y estado interno, es decir, recolectar los datos del mundo exterior y/o interior. Por ejemplo, un sensor ultrasónico es un sensor activo que emite y recibe señales para medir distancias

Los **actuadores** permiten que el robot realice acciones físicas, transformando energía en movimiento o fuerza. Por ejemplo, un motor DC es un actuador eléctrico.

Los **controladores** procesan la información proveniente de los sensores y coordinan las decisiones en tiempo real, enviando instrucciones a los actuadores para realizar las acciones correspondientes.

---

### 2. ¿Cómo se puede estimar la velocidad sin encoders?
**R:** Para hacerlo, se necesita tener una **IMU** tal como la **MPU-6500**. Con el acelerómetro integrado en esta unidad, podemos integrar la aceleración en el tiempo para obtener una estimación de la velocidad lineal. Sin embargo, es susceptible a acumular errores si no se corrige con otros sensores.

---

### 3. ¿Cómo afecta la falta de encoders a la precisión del movimiento? 
**R:** Impide conocer con exactitud la posición y velocidad actual del robot, ya que no existe una medición directa del desplazamiento de cada una de las ruedas. Esto afecta específicamente a la **odometría**, que se encarga de parámetros relativos a la trayectoria y orientación en el espacio.

---

### 4. ¿Qué es PWM y cómo ayuda a controlar la velocidad de los motores?
**R:** Se trata de una técnica que convierte una señal digital en una señal analógica simulada. En motores DC como los que usamos, permite controlar las revoluciones por minuto de cada motor variando el tiempo que la señal permanece activa, lo que regula la potencia entregada a cada motor.

---

### 5. ¿Cómo afecta el control de velocidad a la precisión de la navegación sin encoders?
**R:** Si la velocidad no es constante, para estimar la posición del robot se requiere asumir condiciones ideales o realizar cálculos basados en sensores como IMUs, lo que puede generar fallas en la orientación y en la distancia total recorrida. Se necesita la mayor precisión posible.

---

## Preguntas Parte 2

### 1. ¿Cómo se calcula la velocidad del robot sin encoders usando PWM?
**R:** Se mide el tiempo que el robot tarda en recorrer una distancia conocida y se calcula la velocidad en forma experimental. Luego, se relaciona esa velocidad con el valor de PWM utilizado para futuras estimaciones bajo condiciones similares.

---

### 2. ¿Cómo afectan los intervalos de tiempo a la trayectoria y velocidad del robot?
**R:** Intervalos muy grandes generan respuestas lentas y desvíos en la trayectoria. Si son muy cortos, se capta demasiado ruido. Es importante encontrar un equilibrio y aplicar filtros que suavicen las mediciones del sensor.

---

### 3. ¿Cuáles son las ventajas y desventajas de usar un IMU para ajustar la dirección en lugar de encoders?
**R:** La IMU permite detectar giros e inclinaciones sin depender del movimiento de las ruedas, por lo que no sufre con el deslizamiento. Sin embargo, es más sensible al ruido y acumula errores si no se filtra bien la señal.

---

### 4. ¿Qué efecto tiene la inclinación o el giro en el movimiento del robot, y cómo se corrige con el IMU?
**R:** Cualquier inclinación o giro cambia la orientación del robot y puede desviar su rumbo. El IMU detecta estos cambios y permite corregir la dirección ajustando las ruedas, manteniendo así una trayectoria más precisa.


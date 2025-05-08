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
**R:** Cuando no se tienen los encoders, no se puede obtener la velocidad directamente de las ruedas. Para aquello, se usa un método experimental/manual, es decir, se traza una línea recta y se mide lo que tardó en recorrerla. A partir de estos datos, se calcula la velocidad como distancia dividida por tiempo. Luego, esta velocidad se relaciona con el valor del PWM utilizado, permitiendo una estimación indirecta de la velocidad para futuros movimientos, siempre que se mantengan condiciones similares.

---

### 2. ¿Cómo afectan los intervalos de tiempo a la trayectoria y velocidad del robot?
**R:** Si el intervalo es muy grande, el sistema puede reaccionar tarde a cambios en el entorno o en la orientación, provocando errores acumulados en la trayectoria. Si el intervalo es demasiado corto, puede procesarse demasiada información en poco tiempo, incluyendo ruido o datos inestables. Por eso, es esencial encontrar un equilibrio y aplicar filtros para suavizar las señales, mejorando la estimación de velocidad y orientación, especialmente al usar sensores como una IMU.

---

### 3. ¿Cuáles son las ventajas y desventajas de usar un IMU para ajustar la dirección en lugar de encoders?
**R:** El uso de una IMU para corregir la dirección del robot tiene la ventaja de que no depende del contacto físico con las ruedas o motores, por lo que no sufre problemas de deslizamiento o pérdidas de señal como puede pasar con los encoders. Además, permite medir giros, inclinaciones y aceleraciones del robot en tiempo real. Sin embargo, la IMU suele ser más sensible al ruido y tiende a acumular errores con el tiempo (deriva), por lo que necesita técnicas de filtrado o fusión de sensores (como el filtro de Kalman) para mantener lecturas precisas. Los encoders, por su parte, proporcionan mediciones directas de posición y velocidad, pero dependen de que no haya patinajes ni interrupciones mecánicas.

---

### 4. ¿Qué efecto tiene la inclinación o el giro en el movimiento del robot, y cómo se corrige con el IMU?
**R:** La inclinación o el giro del robot afecta su orientación, y por tanto su trayectoria. Un pequeño cambio en el ángulo puede causar grandes desviaciones si no se detecta a tiempo. La IMU, mediante su giroscopio y acelerómetro, permite medir tanto la velocidad angular como la aceleración en diferentes ejes. Al integrar estas medidas, se puede estimar el ángulo actual del robot (Yaw) y corregir el rumbo ajustando la velocidad de las ruedas. Esto es esencial para mantener una navegación precisa, especialmente cuando no se cuenta con sensores como los encoders para validar el movimiento real de las ruedas.


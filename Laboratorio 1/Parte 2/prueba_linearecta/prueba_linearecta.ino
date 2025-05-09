#include <Wire.h>
#include <MPU9250_asukiaaa.h>

MPU9250_asukiaaa mpu;

// Parámetros de control
float kGyro = 0.1; // Factor de corrección para la rotación (ajustar según necesidad)
float kAccel = 0.2; // Factor de corrección para la inclinación (ajustar según necesidad)

// Variables de estado
float targetAngle = 0.0; // Ángulo deseado (0 es recto, sin inclinación)
float currentAngle = 0.0;
float gyroAngle = 0.0;
float accelAngle = 0.0;

// PWM de motores (ajustar según la velocidad deseada)
int pwmA = 100; // Motor A (izquierda)
int pwmB = 100; // Motor B (derecha)

unsigned long prevTime = 0; // Para controlar el tiempo

// Pines de control de los motores
#define IN1 8
#define IN2 7
#define IN3 5
#define IN4 4

#define ENA 9
#define ENB 10

void setup() {
  Serial.begin(115200);
  Wire.begin();

  mpu.setWire(&Wire);
  mpu.beginAccel();
  mpu.beginGyro();

  if (!mpu.accelUpdate() || !mpu.gyroUpdate()) {
    Serial.println("No se detecta el MPU-6500");
  } else {
    Serial.println("MPU-6500 detectado correctamente");
  }

  // Inicialización de los pines de los motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Dirección inicial: mover hacia adelante
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  
  analogWrite(ENA, pwmA);
  analogWrite(ENB, pwmB);
}

void loop() {
  unsigned long currentTime = millis();
  float dt = (currentTime - prevTime) / 1000.0; // Tiempo transcurrido en segundos
  prevTime = currentTime;

  // Actualizar valores del acelerómetro y giroscopio
  mpu.accelUpdate();
  mpu.gyroUpdate();

  // Obtener valores del acelerómetro y giroscopio
  float accelX = -mpu.accelX();
  float accelY = -mpu.accelY();
  float gyroZ = mpu.gyroZ();

  // Convertir valores del acelerómetro a ángulo (aproximación simple)
  accelAngle = atan2(accelY, accelX) * 180.0 / PI;

  // Actualizar ángulo basado en el giroscopio (integrando la velocidad angular)
  gyroAngle += gyroZ * 0.01; // Multiplicado por dt (ajustar según la frecuencia de actualización)

  // Promediar el ángulo obtenido del acelerómetro y giroscopio
  currentAngle = (accelAngle + gyroAngle) / 2;

  // Mostrar el ángulo y los valores del IMU
  Serial.print("Accel Angle: ");
  Serial.print(accelAngle);
  Serial.print(" | Gyro Angle: ");
  Serial.print(gyroAngle);
  Serial.print(" | Current Angle: ");
  Serial.println(currentAngle);

  // Corrección de la dirección para mover en línea recta
  if (currentAngle > targetAngle + 2) {
    // Desviación hacia la derecha, corregir hacia la izquierda
    pwmA = 100;  // Mantener velocidad del motor A
    pwmB = 90;   // Reducir velocidad del motor B para corregir
    Serial.println("Corrigiendo hacia la izquierda.");
  } else if (currentAngle < targetAngle - 2) {
    // Desviación hacia la izquierda, corregir hacia la derecha
    pwmA = 90;   // Reducir velocidad del motor A para corregir
    pwmB = 100;  // Mantener velocidad del motor B
    Serial.println("Corrigiendo hacia la derecha.");
  } else {
    // Mantener velocidad constante si el robot está recto
    pwmA = 100;  // Velocidad estándar para motor A
    pwmB = 100;  // Velocidad estándar para motor B
    Serial.println("Movimiento recto.");
  }

  // Ajustar la velocidad de los motores
  analogWrite(ENA, pwmA);
  analogWrite(ENB, pwmB);

  delay(100); // Pequeña espera antes de la siguiente medición
}

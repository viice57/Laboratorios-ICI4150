#define IN1 8
#define IN2 7
#define IN3 5
#define IN4 4

#define ENA 9
#define ENB 10

// Parámetros físicos
float r = 0.065;  // radio de las ruedas (m)
float L = 0.135;   // distancia entre ruedas (m)
float K = 0.05;  // factor de conversión PWM -> velocidad (cm/s)

// Estado del robot
float x = 0.0, y = 0.0, theta = 0.0;

// PWM
int pwmA = 70; // entre 0 y 255
int pwmB = 70;

unsigned long prevTime;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);

  // Dirección: avanzar
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);

  prevTime = millis();
}

void loop() {
  analogWrite(ENA, pwmA);
  analogWrite(ENB, pwmB);

  unsigned long currentTime = millis();
  float dt = (currentTime - prevTime) / 1000.0; // segundos
  prevTime = currentTime;

  // Estimar velocidades angulares (rad/s) a partir del PWM (suponiendo relación lineal)
  float omega_L = K * pwmA;  // velocidad angular de la rueda izquierda
  float omega_R = K * pwmB;  // velocidad angular de la rueda derecha

  // Aplicar cinemática diferencial
  float v = (r / 2) * (omega_R + omega_L);  // velocidad lineal media
  float w = (r / L) * (omega_R - omega_L);  // velocidad angular

  // Actualizar posición
  x += v * cos(theta) * dt;
  y += v * sin(theta) * dt;
  theta += w * dt;

  // Imprimir la posición estimada
  Serial.print("x: "); Serial.print(x);
  Serial.print(" y: "); Serial.print(y);
  Serial.print(" theta: "); Serial.println(theta);

  delay(100);  // Ajustar frecuencia de actualización
}

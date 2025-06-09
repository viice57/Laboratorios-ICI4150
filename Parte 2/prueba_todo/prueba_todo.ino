#include <Wire.h>
#include "Adafruit_TCS34725.h"

// ——— Motor A ———
#define IN1 6
#define IN2 7
// ——— Motor B ———
#define IN3 5
#define IN4 4
// ——— Pines de velocidad ———
#define ENA 9
#define ENB 10

// ——— Sensor ultrasónico ———
const int trigPin = 2;
const int echoPin = 3;
const int numDistReadings = 10;
const float distanciaCorreccion = 0.0;
const int umbral = 10;         // cm
// ——— LED indicador ———
const int ledPin = 8;

// ——— Sensor de color TCS34725 ———
Adafruit_TCS34725 tcs(TCS34725_INTEGRATIONTIME_600MS,
                      TCS34725_GAIN_1X);
float prev_fr[3] = {0}, prev_fg[3] = {0}, prev_fb[3] = {0};
int colorIndex = 0;
float a_r = 1.0, b_r = 0.0;
float a_g = 1.0, b_g = 0.0;
float a_b = 1.0, b_b = 0.0;

void setup() {
  Serial.begin(9600);
  // Pines motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Pines sensor ultrasónico y LED
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Inicializar TCS34725
  if (tcs.begin()) {
    Serial.println("Sensor TCS34725 detectado.");
  } else {
    Serial.println("No se encontró TCS34725. Revisa conexiones.");
    while (1);
  }
}

void loop() {
  // — 1) Lectura de distancia —
  float promD = leerDistanciaPromedio();
  Serial.print("Distancia promedio: ");
  Serial.print(promD, 2);
  Serial.println(" cm");

  // Obstacle handling
  if (promD <= umbral) {
    Serial.println("¡Obstáculo detectado!");
    digitalWrite(ledPin, HIGH);
    detener();
  } else {
    digitalWrite(ledPin, LOW);
  }

  // — 2) Detección de color —
  detectarYMostrarColor();

  // — 3) Secuencia de movimiento —
  Serial.println("Adelante");
  moverAdelante();
  delay(2000);

  Serial.println("Atrás");
  moverAtras();
  delay(2000);

  Serial.println("Giro izquierda");
  girarIzquierda();
  delay(2000);

  Serial.println("Giro derecha");
  girarDerecha();
  delay(2000);

  Serial.println("Parado");
  detener();
  delay(5000);
}

// —— Funciones de motor ——
void moverAdelante() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 255);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 255);
}
void moverAtras() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 255);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, 255);
}
void girarIzquierda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 255);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 255);
}
void girarDerecha() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 255);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, 255);
}
void detener() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}

// —— Funciones de sensor ultrasónico ——
float medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long dur = pulseIn(echoPin, HIGH);
  return dur * 0.034f / 2.0f;  // cm
}
float leerDistanciaPromedio() {
  float suma = 0;
  for (int i = 0; i < numDistReadings; i++) {
    suma += medirDistancia();
    delay(50);
  }
  return suma / numDistReadings + distanciaCorreccion;
}

// —— Funciones de detección de color ——
void detectarColor(float r, float g, float b) {
  if (r > g && r > b) Serial.print("ROJO");
  else if (g > r && g > b) Serial.print("VERDE");
  else if (b > r && b > g) Serial.print("AZUL");
  else Serial.print("DESCONOCIDO");
}
void detectarYMostrarColor() {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  if (c == 0) c = 1;
  float fr = (float)r / c;
  float fg = (float)g / c;
  float fb = (float)b / c;

  prev_fr[colorIndex] = fr;
  prev_fg[colorIndex] = fg;
  prev_fb[colorIndex] = fb;
  colorIndex = (colorIndex + 1) % 3;

  float avg_r = (prev_fr[0] + prev_fr[1] + prev_fr[2]) / 3.0;
  float avg_g = (prev_fg[0] + prev_fg[1] + prev_fg[2]) / 3.0;
  float avg_b = (prev_fb[0] + prev_fb[1] + prev_fb[2]) / 3.0;

  float Rcal = a_r * avg_r + b_r;
  float Gcal = a_g * avg_g + b_g;
  float Bcal = a_b * avg_b + b_b;

  Serial.print(" | Raw RGB: ");
  Serial.print(r); Serial.print(","); Serial.print(g); Serial.print(","); Serial.print(b);
  Serial.print(" | Norm RGB: ");
  Serial.print(avg_r, 3); Serial.print(","); Serial.print(avg_g, 3); Serial.print(","); Serial.print(avg_b, 3);
  Serial.print(" | Color: ");
  detectarColor(Rcal, Gcal, Bcal);
  Serial.println();
  delay(200);
}

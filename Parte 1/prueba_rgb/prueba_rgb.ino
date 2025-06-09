#include <Wire.h>
#include "Adafruit_TCS34725.h"

const int trigPin = 2;
const int echoPin = 3;
const int numDistReadings = 10;
const float distanciaCorreccion = 0.0;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(
  TCS34725_INTEGRATIONTIME_600MS,
  TCS34725_GAIN_1X
);

float prev_fr[3] = {0}, prev_fg[3] = {0}, prev_fb[3] = {0};
int colorIndex = 0;

float a_r = 1.0, b_r = 0.0;
float a_g = 1.0, b_g = 0.0;
float a_b = 1.0, b_b = 0.0;

float medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH);
  float distancia = duracion * 0.034f / 2.0f;
  return distancia;
}

void detectarColor(float r, float g, float b) {
  if (r > g && r > b) {
    Serial.print("ROJO");
  } else if (g > r && g > b) {
    Serial.print("VERDE");
  } else if (b > r && b > g) {
    Serial.print("AZUL");
  } else {
    Serial.print("DESCONOCIDO");
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  if (tcs.begin()) {
    Serial.println("Sensor TCS34725 detectado.");
  } else {
    Serial.println("No se encontró el sensor TCS34725.");
    while (1);
  }

  tcs.setInterrupt(false);
}

void loop() {
  float sumaD = 0;
  for (int i = 0; i < numDistReadings; i++) {
    sumaD += medirDistancia();
    delay(50);
  }
  float promD = sumaD / numDistReadings + distanciaCorreccion;
  Serial.print("Distancia promedio: ");
  Serial.print(promD, 2);
  Serial.println(" cm");

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
  Serial.print(r); Serial.print(",");
  Serial.print(g); Serial.print(",");
  Serial.print(b); Serial.print(" C:");
  Serial.print(c);
  Serial.print(" | Norm RGB: ");
  Serial.print(avg_r, 3); Serial.print(",");
  Serial.print(avg_g, 3); Serial.print(",");
  Serial.print(avg_b, 3);
  Serial.print(" | Color: ");
  detectarColor(Rcal, Gcal, Bcal);
  Serial.println();

  delay(200);
}

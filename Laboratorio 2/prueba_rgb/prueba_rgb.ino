#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Crear objeto del sensor con integración larga para mayor precisión
Adafruit_TCS34725 tcs = Adafruit_TCS34725(
  TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

// Variables para filtro de media móvil (ventana N=3)
float prev_fr[3] = {0}, prev_fg[3] = {0}, prev_fb[3] = {0};
int index = 0;

// Coeficientes de calibración (ajústalos si deseas hacer una regresión lineal)
float a_r = 1.0, b_r = 0.0;
float a_g = 1.0, b_g = 0.0;
float a_b = 1.0, b_b = 0.0;

void setup() {
  Serial.begin(9600);
  if (tcs.begin()) {
    Serial.println("Sensor TCS34725 detectado.");
  } else {
    Serial.println("No se encontró el sensor.");
    while (1);
  }
}

void loop() {
  uint16_t r, g, b, c;

  // Obtener datos crudos
  tcs.getRawData(&r, &g, &b, &c);

  // Evitar división por cero
  if (c == 0) c = 1;

  // Normalización RGB
  float fr = (float)r / c;
  float fg = (float)g / c;
  float fb = (float)b / c;

  // Aplicar filtro de media móvil
  prev_fr[index] = fr;
  prev_fg[index] = fg;
  prev_fb[index] = fb;

  float avg_r = (prev_fr[0] + prev_fr[1] + prev_fr[2]) / 3.0;
  float avg_g = (prev_fg[0] + prev_fg[1] + prev_fg[2]) / 3.0;
  float avg_b = (prev_fb[0] + prev_fb[1] + prev_fb[2]) / 3.0;

  index = (index + 1) % 3;

  // Calibración (si hay ajuste de escala o offset)
  float R_cal = a_r * avg_r + b_r;
  float G_cal = a_g * avg_g + b_g;
  float B_cal = a_b * avg_b + b_b;

  // Mostrar resultados
  Serial.print("Raw RGB: ");
  Serial.print(r); Serial.print(", ");
  Serial.print(g); Serial.print(", ");
  Serial.print(b); Serial.print(" | Clear: ");
  Serial.print(c); Serial.print(" | ");

  Serial.print("Normalized RGB: ");
  Serial.print(avg_r, 3); Serial.print(", ");
  Serial.print(avg_g, 3); Serial.print(", ");
  Serial.print(avg_b, 3); Serial.print(" | ");

  Serial.print("Color: ");
  detectarColor(R_cal, G_cal, B_cal, c);
  Serial.println();

  delay(1000);
}

void detectarColor(float r, float g, float b, uint16_t c) {
  // Clasificación empírica básica
  if (c < 300) {
    Serial.print("NEGRO");
  } else if (c > 3000 && r > 0.8 && g > 0.8 && b > 0.8) {
    Serial.print("BLANCO");
  } else if (r > 0.5 && g < 0.4 && b < 0.4) {
    Serial.print("ROJO");
  } else if (g > 0.5 && r < 0.4 && b < 0.4) {
    Serial.print("VERDE");
  } else if (b > 0.5 && r < 0.4 && g < 0.4) {
    Serial.print("AZUL");
  } else {
    Serial.print("DESCONOCIDO");
  }
}

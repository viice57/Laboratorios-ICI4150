#include <Wire.h>
#include "Adafruit_TCS34725.h"

// === HC-SR04 Ultrasonido ===
const int trigPin = 2;
const int echoPin = 3;
const int numDistReadings = 10;    // lecturas a promediar para análisis de precisión
const float distanciaCorreccion = 0.0; // corrección empírica (cm)

// === TCS34725 Sensor RGB ===
// Ventana de integración larga + ganancia 1×
Adafruit_TCS34725 tcs = Adafruit_TCS34725(
  TCS34725_INTEGRATIONTIME_700MS,
  TCS34725_GAIN_1X
);

// Variables para filtro de media móvil (N = 3)
float prev_fr[3] = {0}, prev_fg[3] = {0}, prev_fb[3] = {0};
int colorIndex = 0;

// Calibración lineal (ajusta si deseas)
float a_r = 1.0, b_r = 0.0;
float a_g = 1.0, b_g = 0.0;
float a_b = 1.0, b_b = 0.0;

// --- Función para medir distancia puntual (cm) ---
float medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duracion = pulseIn(echoPin, HIGH);
  float distancia = duracion * 0.034f / 2.0f;  // v_sonido≈343 m/s → 0.034 cm/µs :contentReference[oaicite:1]{index=1}
  return distancia;
}

// --- Clasificación empírica de color ---
void detectarColor(float r, float g, float b, uint16_t c) {
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

void setup() {
  Serial.begin(9600);
  // HC-SR04
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // TCS34725
  if (tcs.begin()) {
    Serial.println("Sensor TCS34725 detectado.");
  } else {
    Serial.println("No se encontró el sensor TCS34725.");
    while (1);
  }
}

void loop() {
  // —— ULTRASONIDO: promedio y varianza ——  
  float sumaD = 0, sumaD2 = 0;
  for (int i = 0; i < numDistReadings; i++) {
    float d = medirDistancia();
    sumaD  += d;
    sumaD2 += d * d;
    delay(50);
  }
  float promD = sumaD / numDistReadings + distanciaCorreccion;
  float varD  = (sumaD2 / numDistReadings) - promD * promD;
  Serial.print("Distancia promedio: ");
  Serial.print(promD, 2);
  Serial.print(" cm  |  Varianza: ");
  Serial.println(varD, 4);

  // —— COLOR: media móvil + calibración ——  
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  if (c == 0) c = 1;  // evita división por cero

  // normalizar
  float fr = (float)r / c;
  float fg = (float)g / c;
  float fb = (float)b / c;

  // almacenar en ventana
  prev_fr[colorIndex] = fr;
  prev_fg[colorIndex] = fg;
  prev_fb[colorIndex] = fb;
  colorIndex = (colorIndex + 1) % 3;

  // promedio de la ventana
  float avg_r = (prev_fr[0] + prev_fr[1] + prev_fr[2]) / 3.0;
  float avg_g = (prev_fg[0] + prev_fg[1] + prev_fg[2]) / 3.0;
  float avg_b = (prev_fb[0] + prev_fb[1] + prev_fb[2]) / 3.0;

  // calibración lineal
  float Rcal = a_r * avg_r + b_r;
  float Gcal = a_g * avg_g + b_g;
  float Bcal = a_b * avg_b + b_b;

  // impresión
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
  detectarColor(Rcal, Gcal, Bcal, c);
  Serial.println();

  delay(1000);
}

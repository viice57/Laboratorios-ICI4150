#include <Wire.h>
#include <MPU9250_asukiaaa.h>      // IMU (acelerómetro + giroscopio)
#include <Adafruit_TCS34725.h>     // Sensor RGB
#include <Servo.h>                 // Control de servomotor

// Ultrasonido (HC-SR04)
#define TRIG_PIN    2
#define ECHO_PIN    3
// Servomotor para barrido
#define SERVO_PIN   11
// Driver L298N motores
#define IN1         6
#define IN2         7
#define IN3         5
#define IN4         4
#define ENA         9
#define ENB         10

// — Umbrales de detección —
// Obstáculo ultrasonido < 10 cm
const float DIST_THRESHOLD   = 10.0;
// Umbrales normalizados RGB (0–1)
const float RED_THRESHOLD    = 0.5;
const float GREEN_THRESHOLD  = 0.5;
const float BLUE_THRESHOLD   = 0.5;

// IMU (filtro complementario)
MPU9250_asukiaaa mpu;
float currentAngle = 0, accelAngle = 0;
const float ALPHA = 0.98;
unsigned long prevTime = 0;

// Sensor RGB (media móvil ventana 3)
Adafruit_TCS34725 tcs(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
const int WINDOW_SIZE = 3;
float windowR[WINDOW_SIZE] = {0}, windowG[WINDOW_SIZE] = {0}, windowB[WINDOW_SIZE] = {0};
int windowIndex = 0;

// Ultrasonido (ventana de 5 mediciones)
const int DIST_WINDOW = 5;
float distWindow[DIST_WINDOW] = {0};
int distIndex = 0;

// Servomotor y control de motores
Servo servo;

// — Funciones auxiliares —

// 1) Medir distancia con HC-SR04 + media móvil
float readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long dur = pulseIn(ECHO_PIN, HIGH);
  float d = dur * 0.034f / 2.0f;  // cm (v sonido ≈ 343 m/s)
  distWindow[distIndex] = d;
  distIndex = (distIndex + 1) % DIST_WINDOW;
  float sum = 0;
  for (int i = 0; i < DIST_WINDOW; i++) sum += distWindow[i];
  return sum / DIST_WINDOW;
}

// 2) Barrido ultrasonoro con servo
void scanUltrasonic(float &minDist, int &angleAtMin) {
  minDist = 1000; angleAtMin = 0;
  for (int ang = 0; ang <= 180; ang += 30) {
    servo.write(ang);
    delay(200);
    float d = readDistance();
    if (d < minDist) { minDist = d; angleAtMin = ang; }
  }
  servo.write(90);
  delay(200);
}

// 3) Actualizar IMU + filtro complementario
void updateIMU() {
  unsigned long now = millis();
  float dt = (now - prevTime) / 1000.0;
  prevTime = now;
  mpu.accelUpdate();
  mpu.gyroUpdate();
  accelAngle = atan2(-mpu.accelY(), -mpu.accelX()) * 180.0 / PI;
  currentAngle = ALPHA * (currentAngle + mpu.gyroZ() * dt)
               + (1 - ALPHA) * accelAngle;
}

// 4) Lectura color + media móvil
void readColor(float &rAve, float &gAve, float &bAve, uint16_t &c) {
  uint16_t r, g, b;
  tcs.getRawData(&r, &g, &b, &c);
  float fr = (float)r / c, fg = (float)g / c, fb = (float)b / c;
  windowR[windowIndex] = fr;
  windowG[windowIndex] = fg;
  windowB[windowIndex] = fb;
  windowIndex = (windowIndex + 1) % WINDOW_SIZE;
  rAve = gAve = bAve = 0;
  for (int i = 0; i < WINDOW_SIZE; i++) {
    rAve += windowR[i];
    gAve += windowG[i];
    bAve += windowB[i];
  }
  rAve /= WINDOW_SIZE;
  gAve /= WINDOW_SIZE;
  bAve /= WINDOW_SIZE;
}

// 5) Control simple de motores
void drive(int leftPWM, int rightPWM, bool forward = true) {
  digitalWrite(IN1, forward);
  digitalWrite(IN2, !forward);
  digitalWrite(IN3, forward);
  digitalWrite(IN4, !forward);
  analogWrite(ENA, leftPWM);
  analogWrite(ENB, rightPWM);
}

// 6) Maniobra según color detectado
void handleColorDecision(float r, float g, float b) {
  if (r > RED_THRESHOLD && g < GREEN_THRESHOLD && b < BLUE_THRESHOLD) {
    // rojo → gira izquierda
    drive(80, 100, true);
  }
  else if (g > GREEN_THRESHOLD && r < RED_THRESHOLD && b < BLUE_THRESHOLD) {
    // verde → recto
    drive(100, 100, true);
  }
  else if (b > BLUE_THRESHOLD && r < RED_THRESHOLD && g < GREEN_THRESHOLD) {
    // azul → gira derecha
    drive(100, 80, true);
  }
  else {
    // desconocido → retrocede
    drive(80, 80, false);
    delay(300);
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // IMU
  mpu.setWire(&Wire);
  mpu.beginAccel();
  mpu.beginGyro();
  prevTime = millis();

  // Sensor RGB
  if (!tcs.begin()) {
    Serial.println("Error TCS34725");
    while (1);
  }

  // Servomotor
  servo.attach(SERVO_PIN);

  // Ultrasonido
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void loop() {
  // 1) Escaneo ultrasonoro
  float dMin; int angMin;
  scanUltrasonic(dMin, angMin);

  // 2) Si obstáculo cercano
  if (dMin < DIST_THRESHOLD) {
    drive(0, 0, false);
    float r, g, b; uint16_t c;
    readColor(r, g, b, c);
    handleColorDecision(r, g, b);
    return;
  }

  // 3) Si libre: mantener rumbo con IMU y avanzar
  updateIMU();
  float error = currentAngle;            // objetivo = 0°
  int corr = constrain((int)(error * 2), -30, 30);
  drive(90 - corr, 90 + corr, true);
}

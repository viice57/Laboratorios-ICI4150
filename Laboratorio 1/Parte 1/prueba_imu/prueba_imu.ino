#include <Wire.h>
#include <MPU9250_asukiaaa.h>

MPU9250_asukiaaa mpu;

float accelBiasX = 0, accelBiasY = 0, accelBiasZ = 0;
float gyroBiasX = 0, gyroBiasY = 0, gyroBiasZ = 0;

#define N 10
float ax_buffer[N], ay_buffer[N], az_buffer[N];
int index = 0;

float filter(float buffer[], float new_value) {
  buffer[index] = new_value;
  float sum = 0;
  for (int i = 0; i < N; i++) sum += buffer[i];
  return sum / N;
}

// Para integración del giroscopio
unsigned long lastTime = 0;
float yaw = 0; // rotación acumulada

// Filtro complementario
float pitch = 0, roll = 0;
const float alpha = 0.98; // peso del giroscopio

void setup() {
  Serial.begin(9600);
  Wire.begin();

  mpu.setWire(&Wire);
  mpu.beginAccel();
  mpu.beginGyro();
  delay(1000);

  for (int i = 0; i < N; i++) {
    ax_buffer[i] = 0; ay_buffer[i] = 0; az_buffer[i] = 0;
  }

  Serial.println("Calibrando... mantén el sensor quieto y plano");

  const int muestras = 500;
  for (int i = 0; i < muestras; i++) {
    mpu.accelUpdate();
    mpu.gyroUpdate();

    accelBiasX += mpu.accelX();
    accelBiasY += mpu.accelY();
    accelBiasZ += mpu.accelZ();

    gyroBiasX += mpu.gyroX();
    gyroBiasY += mpu.gyroY();
    gyroBiasZ += mpu.gyroZ();

    delay(5);
  }

  accelBiasX /= muestras;
  accelBiasY /= muestras;
  accelBiasZ = (accelBiasZ / muestras) - 1.0;
  gyroBiasX /= muestras;
  gyroBiasY /= muestras;
  gyroBiasZ /= muestras;

  lastTime = millis();
  Serial.println("Calibración completa.");
}

void loop() {
  mpu.accelUpdate();
  mpu.gyroUpdate();

  float ax = -(mpu.accelX() - accelBiasX) * 9.81;
  float ay = -(mpu.accelY() - accelBiasY) * 9.81;
  float az = (mpu.accelZ() - accelBiasZ) * 9.81;

  float gx = -(mpu.gyroX() - gyroBiasX); // en °/s
  float gy = -(mpu.gyroY() - gyroBiasY);
  float gz = mpu.gyroZ() - gyroBiasZ;

  float ax_f = filter(ax_buffer, ax);
  float ay_f = filter(ay_buffer, ay);
  float az_f = filter(az_buffer, az);
  index = (index + 1) % N;

  // Tiempo transcurrido
  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;
  lastTime = now;

  // Cálculo de pitch y roll con acelerómetro
  float pitchAcc = atan2(ax_f, sqrt(ay_f * ay_f + az_f * az_f)) * 180.0 / PI;
  float rollAcc  = atan2(ay_f, sqrt(ax_f * ax_f + az_f * az_f)) * 180.0 / PI;

  // Filtro complementario (mezcla acelerómetro y giroscopio)
  pitch = alpha * (pitch + gx * dt) + (1 - alpha) * pitchAcc;
  roll  = alpha * (roll + gy * dt) + (1 - alpha) * rollAcc;

  // Yaw solo con integración del giroscopio
  yaw += gz * dt;

  // Mostrar resultados
  Serial.print("Pitch: "); Serial.print(pitch, 2); Serial.print("°\t");
  Serial.print("Roll: "); Serial.print(roll, 2); Serial.print("°\t");
  Serial.print("Yaw: "); Serial.print(yaw, 2); Serial.println("°");

  delay(5000);
}

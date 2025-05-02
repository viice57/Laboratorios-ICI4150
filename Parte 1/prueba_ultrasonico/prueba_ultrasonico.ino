#define TRIG_PIN 2
#define ECHO_PIN 3

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(9600);
}

void loop() {
  long tiempo;
  float distancia_cm;

  // Partir desde un estado bajo
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Pulso al trigger
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Medir duración del eco
  tiempo = pulseIn(ECHO_PIN, HIGH);

  // Calcular distancia en centimetros
  distancia_cm = 0.0343 * tiempo / 2;

  Serial.print("Distancia: ");
  Serial.print(distancia_cm);
  Serial.println(" cm");

  delay(1000);
}

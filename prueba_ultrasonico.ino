#define TRIG_PIN 2
#define ECHO_PIN 3

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  long duration;
  float distance_cm;

  // Pulso al trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Medir duración del eco
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calcular distancia (cm)
  distance_cm = duration * 0.034 / 2;

  Serial.print("Distancia: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  delay(500);
}

// Motor A
#define IN1 8
#define IN2 7

//Motor B
#define IN3 5
#define IN4 4

void setup() {
  // Definir pines de salida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  Serial.println("Adelante");
  moverAdelante();
  delay(2000); // Espera de 2s.

  Serial.println("Atrás");
  moverAtras();
  delay(2000); // Espera de 2s.

  Serial.println("Giro izquierda");
  girarIzquierda();
  delay(2000); // Espera de 2s.

  Serial.println("Giro derecha");
  girarDerecha();
  delay(2000); // Espera de 2s.

  Serial.println("Parado");
  detener();
  delay(5000); // Espera de 3s.
}

void moverAdelante() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moverAtras() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void girarIzquierda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void girarDerecha() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void detener() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

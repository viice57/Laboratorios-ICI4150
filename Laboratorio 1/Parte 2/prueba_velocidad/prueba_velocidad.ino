// Pines de control de dirección
#define IN1 8
#define IN2 7
#define IN3 5
#define IN4 4

#define ENA 9 
#define ENB 10

void setup() {
  // Pines de dirección
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Pines de velocidad
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // Velocidad 1 (lenta)
  Serial.println("Velocidad lenta");
  moverAdelante(80, 80);  // PWM bajo
  delay(3000);

  detener();
  delay(1000);

  // Velocidad 2 (media)
  Serial.println("Velocidad media");
  moverAdelante(150, 150);
  delay(3000);

  detener();
  delay(1000);

  // Velocidad 3 (rápida)
  Serial.println("Velocidad alta");
  moverAdelante(230, 230);
  delay(3000);

  detener();
  delay(5000);  // Pausa más larga
}

// Movimiento hacia adelante con PWM variable
void moverAdelante(int pwmL, int pwmR) {
  // Motor izquierdo
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, pwmL);

  // Motor derecho
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, pwmR);
}

// Detener motores
void detener() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

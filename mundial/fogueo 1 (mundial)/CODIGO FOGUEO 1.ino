// Motor LEGO NXT con Arduino + DRV8871
// Control básico de giro + lectura de encoder

#include <Servo.h>

int corto1 = 1000;
int largo1 = 3000;

Servo servo;

int PHASE = 5;   // dirección
int ENABLE = 6;  // velocidad (PWM)

int ENC_A = 7;   // encoder canal A
int ENC_B = 8;   // encoder canal B

volatile long encoderCount = 0;

void encoderISR() {
  int A = digitalRead(ENC_A);
  int B = digitalRead(ENC_B);
  if (A == B) {
    encoderCount++;  // un sentido
  } else {
    encoderCount--;  // otro sentido
  }
}

void setup() {

  Serial.begin(115200);

  pinMode(PHASE, OUTPUT);
  pinMode(ENABLE, OUTPUT);

  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENC_A), encoderISR, CHANGE);
 
  servo.attach(3);
  servo.write(98);  
}

void turnR(){

  servo.write(138);
  delay(800);
  servo.write(98);

}

void turnL(){

  servo.write(98);
  delay(800);
  servo.write(138);

}

void corto(){
  delay(corto1); 
}

void largo(){
  delay(largo1);
}

void mediavuelta(){
  turnR();
  turnR();
}

void loop() {
  // Girar motor adelante
  digitalWrite(ENABLE, LOW);
  analogWrite(PHASE, 250);
  //delay(2000);
  //digitalWrite(ENABLE, LOW);
  //analogWrite(PHASE, 250);
  //delay(2000);

  corto();
  for(int i = 0; i < 4; i++){
    turnR();
    largo();
  }
  turnR();
  corto();

  mediavuelta();

  corto();
  for(int i = 0; i < 4; i++){
    turnL();
    largo();
  }
  turnL();
  corto();

  // Parar
  //analogWrite(ENABLE, 0);
  //delay(1000);

  // Girar motor atrás
  //digitalWrite(PHASE, LOW);
  //analogWrite(ENABLE, 255);
  //delay(2000);

  // Parar
  analogWrite(ENABLE, 0);
  analogWrite(PHASE, 0);
  // Mostrar cuántos ticks leyó el encoder
  //Serial.print("Encoder count: ");
  //Serial.println(encoderCount);

  delay(15000);
}


#include <Servo.h>

// Sensor pins
const int trigCenter = 49;
const int echoCenter = 48;
const int trigLeft = 27;
const int echoLeft = 26;
const int trigRight = 51;
const int echoRight = 50;

// Button and motor control
const int startButtonPin = 5;
const int motorControlPin = 14;
bool motorStarted = false;

Servo barrier;

int lastLeft = 0;
int lastRight = 0;
int currentAngle = 90; // Servo starts at 90 (straight)

void setup() {
  // Set up sensor pins
  pinMode(trigCenter, OUTPUT);
  pinMode(echoCenter, INPUT);
  pinMode(trigLeft, OUTPUT);
  pinMode(echoLeft, INPUT);
  pinMode(trigRight, OUTPUT);
  pinMode(echoRight, INPUT);

  // Button and motor control
  pinMode(startButtonPin, INPUT_PULLUP); // Active LOW button
  pinMode(motorControlPin, OUTPUT);
  digitalWrite(motorControlPin, LOW);    // Motor off initially

  // Servo setup
  barrier.attach(52);
  barrier.write(currentAngle); // Start with barrier down
  Serial.begin(9600);
}

// Measure distance from one ultrasonic sensor
int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // Timeout after 30ms
  int distance = duration * 0.034 / 2;
  return distance;
}

void loop() {
  // Check button press to start motor
  if (!motorStarted && digitalRead(startButtonPin) == LOW) {
    motorStarted = true;
    digitalWrite(motorControlPin, HIGH);
    Serial.println("Motor started!");
    delay(500); // debounce delay
  }

  if (!motorStarted) {
    // Wait for button press, skip the rest
    return;
  }
  // Ultrasonic sensor logic
  int distanceF = getDistance(trigCenter, echoCenter);
  delay(10);
  int distanceL = getDistance(trigLeft, echoLeft);
  delay(10);
  int distanceR = getDistance(trigRight, echoRight);
  delay(10);

  Serial.print("F: "); Serial.print(distanceF);
  Serial.print(" cm, L45: "); Serial.print(distanceL);
  Serial.print(" cm, R45: "); Serial.println(distanceR);

  // Sudden obstacle detection
  int deltaL = lastLeft - distanceL;
  int deltaR = lastRight - distanceR;

  lastLeft = distanceL;
  lastRight = distanceR;

  // Wall in front
  if (distanceF <= 30) {
    if (distanceL > distanceR) {
      barrier.write(135); // Turn right
      currentAngle = 135;
      Serial.println("Wall in FRONT → Turning RIGHT");
    } else {
      barrier.write(45);  // Turn left
      currentAngle = 45;
      Serial.println("Wall in FRONT → Turning LEFT");
    }
  }

  int delta = round(distanceL - distanceR);
  // Normal behavior
  
  if (distanceF > 50 && delta > 0){
    int angle = 90 + delta;
    if(angle < 135){ 
    barrier.write(angle);
    currentAngle = angle;
    } // Turn right

    if(angle > 135){
      barrier.write(135);
      currentAngle = 135;
    }

    if(angle > 45){ 
      barrier.write(angle);
      currentAngle = angle;
    } // Turn right

    if(angle < 45){
      barrier.write(45);
      currentAngle = 45;
    }
  }

  delay(50);
}
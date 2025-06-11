// 新城市黨.ino
// 請根據 car.ino 的結構自行擴充功能

#include <Arduino.h>

// 馬達與感測器腳位設定
const int ENA = 3;
const int IN1 = 5;
const int IN2 = 6;
const int IN3 = 9;
const int IN4 = 10;
const int ENB = 11;
const int motorspeed = 130;
const int irSensorLeft = 8;
const int irSensorRight = 7;
const int trigPin = 12;
const int echoPin = 13;
const int ledLeft = A4;
const int ledRight = A5;
const int avoidLeft = A1;
const int avoidRight = A3;

void forward() {
  analogWrite(ENA, motorspeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, motorspeed);
  digitalWrite(ledLeft, LOW);
  digitalWrite(ledRight, LOW);
}

void backward() {
  analogWrite(ENA, motorspeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, motorspeed);
}

void turnLeft() {
  analogWrite(ENA, 0);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, motorspeed);
  digitalWrite(ledLeft, HIGH);
  digitalWrite(ledRight, LOW);
}

void turnRight() {
  analogWrite(ENA, motorspeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENB, 0);
  digitalWrite(ledLeft, LOW);
  digitalWrite(ledRight, HIGH);
}

void stopMotor() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(ledLeft, LOW);
  digitalWrite(ledRight, LOW);
}

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(irSensorLeft, INPUT);
  pinMode(irSensorRight, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledLeft, OUTPUT);
  pinMode(ledRight, OUTPUT);
}

void loop() {
  bool LS = digitalRead(irSensorLeft);
  bool RS = digitalRead(irSensorRight);
  bool AL = digitalRead(avoidLeft);
  bool AR = digitalRead(avoidRight);

  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 20000);
  distance = duration * 0.034 / 2;

  if (distance > 0 && distance < 15 || AL == LOW || AR == LOW) {
    stopMotor();
    delay(300);
    backward();
    delay(300);
    if (AL == LOW) {
      turnRight();
      delay(600);
    } else if (AR == LOW) {
      turnLeft();
      delay(600);
    } else {
      if (random(2) == 0) {
        turnLeft();
      } else {
        turnRight();
      }
      delay(800);
    }
    forward();
    delay(500);
    stopMotor();
    delay(100);
    return;
  }

  if (LS == LOW && RS == LOW) {
    stopMotor();
    delay(500);
  } else if (LS == HIGH && RS == LOW) {
    turnLeft();
    delay(100);
  } else if (LS == LOW && RS == HIGH) {
    turnRight();
    delay(100);
  } else if (LS == HIGH && RS == HIGH) {
    forward();
    delay(100);
  }
  delay(50);
}

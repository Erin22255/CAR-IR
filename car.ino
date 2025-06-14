// car.ino
// 馬達控制腳位設定
#include <Arduino.h>
const int ENA = 3;   // PWM
const int IN1 = 5;   // PWM
const int IN2 = 6;   // PWM
const int IN3 = 9;   // PWM
const int IN4 = 10;  // PWM
const int ENB = 11;  // PWM

const int motorspeed = 130; // 可自行調整速度

// 紅外線循跡感測器腳位設定
const int irSensorLeft = 8;
const int irSensorRight = 7;
const int trigPin = 12;    // 超音波Trig
const int echoPin = 13;   // 超音波Echo

const int ledLeft = A4;
const int ledRight = A5;

// 新增紅外線避障感測器腳位
const int avoidLeft = A1;  // 左避障
const int avoidRight = A3; // 右避障

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
  digitalWrite(ledLeft, HIGH);   // 左轉亮左燈
  digitalWrite(ledRight, LOW);
}

void turnRight() {
  analogWrite(ENA, motorspeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENB, 0);
  digitalWrite(ledLeft, LOW);
  digitalWrite(ledRight, HIGH);  // 右轉亮右燈
}

void spinLeft() {
  analogWrite(ENA, motorspeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, motorspeed);
}

void spinRight() {
  analogWrite(ENA, motorspeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, motorspeed);
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
  // 1. 讀取所有感測器
  bool LS = digitalRead(irSensorLeft);   // 循跡左
  bool RS = digitalRead(irSensorRight);  // 循跡右
  bool AL = digitalRead(avoidLeft);      // 左避障
  bool AR = digitalRead(avoidRight);     // 右避障

  // 超音波距離
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 20000);
  distance = duration * 0.034 / 2;

  // 2. 障礙判斷
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

  // 3. 循跡判斷
  if (LS == LOW && RS == LOW) {
    stopMotor(); // 十字路口，停下來
    delay(500);
  } else if (LS == HIGH && RS == LOW) {
    turnLeft(); // 左轉校正
    delay(100);
  } else if (LS == LOW && RS == HIGH) {
    turnRight(); // 右轉校正
    delay(100);
  } else if (LS == HIGH && RS == HIGH) {
    forward(); // 前進
    delay(100);
  }
  delay(50); // 延時50ms後繼續循環
}

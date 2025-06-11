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
  bool LS = digitalRead(irSensorLeft);
  bool RS = digitalRead(irSensorRight);

  // 1. 先做循跡判斷（修正：偵測到黑線為 HIGH，白色為 LOW）
  if (LS == HIGH && RS == HIGH) {
    stopMotor(); // 十字或交叉情況，先停下來
    delay(50);
    return;
  }
  else if (LS == LOW && RS == LOW) {
    forward();   // 中間在線上，正常前進
    delay(10);
    // 不 return，繼續檢查避障
  }
  else if (LS == HIGH && RS == LOW) {
    turnRight(); // 左偏出線，往右修正
    delay(10);
    return;
  }
  else if (LS == LOW && RS == HIGH) {
    turnLeft();  // 右偏出線，往左修正
    delay(10);
    return;
  }

  // 2. 再做超音波避障
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 20000); // 最多等20ms
  distance = duration * 0.034 / 2; // 計算距離（公分）

  if (distance > 0 && distance < 15) { // 前方小於15cm有障礙物
    stopMotor();
    delay(500);
    backward();
    delay(200);
    spinRight();
    delay(300);
    stopMotor();
    delay(200);
    return;
  }
  delay(50); // 短暫延遲穩定偵測
}
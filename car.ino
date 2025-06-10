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

  // 偵測到黑線為 LOW、白色為 HIGH
  if (LS == LOW && RS == LOW) {
    stopMotor(); // 十字或交叉情況，先停下來
    delay(500); // 增加延遲
  }
  else if (LS == HIGH && RS == HIGH) {
    forward();   // 中間在線上，正常前進
    delay(100); // 增加延遲
  }
  else if (LS == LOW && RS == HIGH) {
    turnRight(); // 左偏出線，往右修正
    delay(100); // 增加延遲
  }
  else if (LS == HIGH && RS == LOW) {
    turnLeft();  // 右偏出線，往左修正
    delay(100); // 增加延遲
  }
  delay(10); // 短暫延遲穩定偵測
}

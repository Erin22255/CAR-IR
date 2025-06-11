#include <Arduino.h>

const int SENSOR_LEFT = A0;
const int SENSOR_RIGHT = A2;

const int IN1 = 4;
const int IN2 = 5;
const int IN3 = 6;
const int IN4 = 7;
const int ENA = 9;
const int ENB = 3;
// 馬達速度設定
const int SPEED_FORWARD = 100;   // 直行速度（原150，調低）
const int SPEED_TURN_FAST = 120; // 轉彎外側輪快（原180，調低）
const int SPEED_TURN_SLOW = 50;  // 轉彎內側輪慢（原80，調低）
const int SPEED_STOP = 0;        // 停止

void setup() {
  pinMode(SENSOR_LEFT, INPUT);
  pinMode(SENSOR_RIGHT, INPUT);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
}

void loop() {
  int L = digitalRead(SENSOR_LEFT);
  int R = digitalRead(SENSOR_RIGHT);

  if (L == 0 && R == 0) {         // 中線上，直走
    motor(SPEED_FORWARD, SPEED_FORWARD);
    delay(80);
    motor(SPEED_STOP, SPEED_STOP);
  } else if (L == 0 && R == 1) {  // 偏右，原地左轉
    motor(-SPEED_TURN_SLOW, SPEED_TURN_SLOW); // 左輪後退，右輪前進
    delay(80);
    motor(SPEED_STOP, SPEED_STOP);
  } else if (L == 1 && R == 0) {  // 偏左，原地右轉
    motor(SPEED_TURN_SLOW, -SPEED_TURN_SLOW); // 左輪前進，右輪後退
    delay(80);
    motor(SPEED_STOP, SPEED_STOP);
  } else {                        // 離線，停車
    motor(SPEED_STOP, SPEED_STOP);
    delay(500);
  }
}

// 支援正轉與原地急轉（負值為後退）
void motor(int leftSpeed, int rightSpeed) {
  // 左馬達
  if (leftSpeed > 0) {
    analogWrite(ENA, leftSpeed);
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); // 正轉
  } else if (leftSpeed < 0) {
    analogWrite(ENA, -leftSpeed);
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); // 反轉
  } else {
    analogWrite(ENA, 0);
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  }
  // 右馬達
  if (rightSpeed > 0) {
    analogWrite(ENB, rightSpeed);
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); // 正轉
  } else if (rightSpeed < 0) {
    analogWrite(ENB, -rightSpeed);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); // 反轉
  } else {
    analogWrite(ENB, 0);
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  }
}

// smart_car.ino
// Arduino 自走車循跡與避障範例程式

// 馬達與感測器引腳定義
const int leftMotorForward = 3;
const int leftMotorBackward = 4;
const int rightMotorForward = 5;
const int rightMotorBackward = 6;
const int irSensorLeft = 7;
const int irSensorRight = 8;
const int trigPin = 9;    // 超音波Trig
const int echoPin = 10;   // 超音波Echo

void setup() {
  pinMode(leftMotorForward, OUTPUT);
  pinMode(leftMotorBackward, OUTPUT);
  pinMode(rightMotorForward, OUTPUT);
  pinMode(rightMotorBackward, OUTPUT);
  pinMode(irSensorLeft, INPUT);
  pinMode(irSensorRight, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int leftIR = digitalRead(irSensorLeft);
  int rightIR = digitalRead(irSensorRight);
  long distance = getDistance();

  if (distance > 0 && distance < 20) {
    stopCar();
    delay(500);
    turnRight();
    delay(400);
    return;
  }

  if (leftIR == LOW && rightIR == LOW) {
    forward();
  } else if (leftIR == LOW && rightIR == HIGH) {
    turnLeft();
  } else if (leftIR == HIGH && rightIR == LOW) {
    turnRight();
  } else {
    stopCar();
  }
}

void forward() {
  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(rightMotorBackward, LOW);
}

void turnLeft() {
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(leftMotorBackward, HIGH);
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(rightMotorBackward, LOW);
}

void turnRight() {
  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(rightMotorBackward, HIGH);
}

void stopCar() {
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(rightMotorBackward, LOW);
}

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 20000);
  long distance = duration * 0.034 / 2;
  return distance;
}

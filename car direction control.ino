const int ENA=3, IN1=2, IN2=4;
const int ENB=5, IN3=6, IN4=7;
const int IR_left = 8, IR_right = 9;
const int LED_LEFT = 12,LED_RIGHT = 13;

void setup() {
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(IR_left, INPUT); pinMode(IR_right, INPUT);
  Serial.begin(9600);
}

void loop() {
  int l = digitalRead(IR_left);
  int r = digitalRead(IR_right);

  if (l == LOW && r == LOW) {
    Serial.println("go through");
    goForward();
  } else if (l == HIGH && r == LOW) {
    Serial.println("turn right");
    turnRight();
  } else if (l == LOW && r == HIGH) {
    Serial.println("turn left");
    turnLeft();
  } else {
    Serial.println("stop");
    stopMotors();
    delay(500);
  }
  delay(20); // 更快反應
}

// 馬達控制函式
void goForward() {
  analogWrite(ENA, 100); // 左馬達速度
  analogWrite(ENB, 100); // 右馬達速度
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight() {
  analogWrite(ENA, 0);    // 停止左馬達
  analogWrite(ENB, 100);  // 右馬達轉動
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  analogWrite(ENA, 100);  // 左馬達轉動
  analogWrite(ENB, 0);    // 停止右馬達
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

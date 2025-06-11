

// ===== 硬體引腳定義 =====
const int ENA = 3;    // 左馬達PWM
const int IN1 = 5;    // 左馬達方向1
const int IN2 = 6;    // 左馬達方向2
const int IN3 = 9;    // 右馬達方向1
const int IN4 = 10;   // 右馬達方向2
const int ENB = 11;   // 右馬達PWM


const int irSensorLeft = 8;   // 左循跡感測器
const int irSensorRight = 7;  // 右循跡感測器
const int trigPin = 12;       // 超音波Trig
const int echoPin = 13;       // 超音波Echo
const int ledLeft = A4;       // 左LED指示燈
const int ledRight = A5;      // 右LED指示燈


// ===== 參數設定 =====
const int motorspeed = 130;       // 基礎速度
const int turnSpeed = 180;
const int turnSpeed2 = 150;       // 轉向速度
const int turnSpeed1 = 60;


const int motorspeed2 = 100;
const int obstacleDistance=10;
// ===== 馬達控制函數 =====
void forward() {
  analogWrite(ENA, motorspeed2);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, motorspeed);
  digitalWrite(ledLeft, LOW);
  digitalWrite(ledRight, LOW);
  Serial.println("動作：前進");
}


void backward() {
  analogWrite(ENA, motorspeed2);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, motorspeed);
  Serial.println("動作：後退");
}


void turnLeft() {
  analogWrite(ENA, turnSpeed2);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, turnSpeed1);  // 右輪速度降低
  digitalWrite(ledLeft, HIGH);
  digitalWrite(ledRight, LOW);
  Serial.println("動作：左轉");
}


void turnRight() {
  analogWrite(ENA, turnSpeed1);  // 左輪速度降低
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, turnSpeed);
  digitalWrite(ledLeft, LOW);
  digitalWrite(ledRight, HIGH);
  Serial.println("動作：右轉");
}


void stopMotor() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(ledLeft, LOW);
  digitalWrite(ledRight, LOW);
  Serial.println("動作：停止");
}


// ===== 超音波距離測量 =====
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 20000);
  if(duration == 0) return 999; // 超時返回大值
  return duration * 0.034 / 2;
}


// ===== 初始化設定 =====
void setup() {
  // 馬達引腳初始化
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
 
  // 感測器引腳初始化
  pinMode(irSensorLeft, INPUT);
  pinMode(irSensorRight, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledLeft, OUTPUT);
  pinMode(ledRight, OUTPUT);
 
  Serial.begin(9600);
  Serial.println("系統初始化完成");
}


// ===== 主控制循環 =====
void loop() {
  // 1. 讀取循跡感測器狀態（注意：HIGH=白線，LOW=黑線）
  bool leftSensor = digitalRead(irSensorLeft);
  bool rightSensor = digitalRead(irSensorRight);
 
  // 2. 讀取超音波距離
  int distance = getDistance();
 
  // 調試信息
  Serial.print("左感測: ");
  Serial.print(leftSensor);
  Serial.print(" 右感測: ");
  Serial.print(rightSensor);
  Serial.print(" 距離: ");
  Serial.print(distance);
  Serial.println("cm");


  // 3. 避障判斷（最高優先級）
  if (distance > 0 && distance < obstacleDistance) {
    Serial.println("偵測到障礙物！");
    stopMotor();
    delay(500);
    backward();
    delay(300);
    turnRight();
    delay(400);
    return; // 本回合結束
  }


  // 4. 循跡控制（修正後的邏輯）
  if (leftSensor == LOW && rightSensor == LOW) {
    forward();   // 兩側都是白線（在線上）
  }
  else if (leftSensor == HIGH && rightSensor == LOW) {
    turnRight(); // 左側檢測到黑線（偏左），向右修正
  }
  else if (leftSensor == LOW && rightSensor == HIGH) {
    turnLeft();  // 右側檢測到黑線（偏右），向左修正
  }
  else {
    stopMotor(); // 兩側都檢測到黑線（十字路口）
    delay(50);
  }
 
  delay(50); // 主循環延時
}

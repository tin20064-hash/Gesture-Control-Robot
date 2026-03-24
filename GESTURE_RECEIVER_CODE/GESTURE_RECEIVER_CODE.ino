#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial BTSerial(2, 3); // RX=2, TX=3
Servo myservo;

// Khai báo chân L298N
const int IN1 = 4; const int IN2 = 5;
const int IN3 = 6; const int IN4 = 7;
const int ENA = 8; const int ENB = 9;

// Khai báo chân cảm biến siêu âm
const int TRIG_PIN = A2;
const int ECHO_PIN = A3;
const int SERVO_PIN = 10;

int tocDo = 200; 
bool isAutoMode = false;

void setup() {
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  myservo.attach(SERVO_PIN);
  myservo.write(90); // Cho servo nhìn thẳng

  dungIm();
  Serial.begin(9600);
  BTSerial.begin(9600);
  Serial.println("HE THONG DA SAN SANG!");
}

void loop() {
  // 1. Kiểm tra lệnh từ Bluetooth
  if (BTSerial.available()) {
    char command = BTSerial.read();
    if (command == 'a') {
      isAutoMode = true;
      Serial.println("CHE DO: TU DONG");
    } else if (command == 's' || command == 'S') {
      isAutoMode = false;
      dungIm();
      Serial.println("CHE DO: DIEU KHIEN TAY - DUNG");
    } else if (!isAutoMode) {
      handleManualCommand(command);
    }
  }

  // 2. Nếu đang ở chế độ tự động
  if (isAutoMode) {
    long khoangCach = docKhoangCach();

    if (khoangCach <= 15 && khoangCach > 0) { // Khoảng cách an toàn 
      dungIm();
      delay(500);
      chayLui(); // Lui lại một chút để có không gian quay
      delay(300);
      dungIm();
      
      handleAvoidance(); // Xử lý tìm đường
    } else {
      chayTien();
    }
  }
}

// --- HÀM XỬ LÝ TRÁNH VẬT CẢN ---
void handleAvoidance() {
  int distanceLeft, distanceRight;

  // Quay sang phải
  myservo.write(10);
  delay(500);
  distanceRight = docKhoangCach();
  
  // Quay sang trái
  myservo.write(170);
  delay(500);
  distanceLeft = docKhoangCach();
  
  // Trả về thẳng
  myservo.write(90);
  delay(500);

  if (distanceLeft > distanceRight) {
    reTrai();
    delay(500); // Thời gian quay đủ để hướng xe sang trái
  } else {
    rePhai();
    delay(500);
  }
  dungIm();
}

// --- HÀM ĐỌC KHOẢNG CÁCH ---
long docKhoangCach() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

// --- ĐIỀU KHIỂN TAY ---
void handleManualCommand(char cmd) {
  switch (cmd) {
    case 'F': chayTien(); break;
    case 'B': chayLui(); break;
    case 'L': reTrai(); break;
    case 'R': rePhai(); break;
    case 'S': dungIm(); break;
  }
}

// --- CÁC HÀM DI CHUYỂN ---
void chayTien() {
  analogWrite(ENA, tocDo); analogWrite(ENB, tocDo);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
}
void chayLui() {
  analogWrite(ENA, tocDo); analogWrite(ENB, tocDo);
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}
void reTrai() {
  analogWrite(ENA, tocDo); analogWrite(ENB, tocDo);
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); 
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
}
void rePhai() {
  analogWrite(ENA, tocDo); analogWrite(ENB, tocDo);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}
void dungIm() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(2, 3); // Chân RX=2, TX=3 kết nối HC-05

// Khai báo chân chiều quay L298N
const int IN1 = 4;
const int IN2 = 5;
const int IN3 = 6;
const int IN4 = 7;

// CẢNH BÁO: Chân 8 không có PWM, sẽ không điều chỉnh được tốc độ mượt mà
const int ENA = 8; 
const int ENB = 9; // Chân 9 có PWM (có dấu ~)

// Nếu dùng chân 8, khuyên bạn nên để nguyên mức 255
int tocDo = 255; 

void setup() {
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  // Ép xe đứng im ngay khi vừa cấp nguồn
  dungIm();

  Serial.begin(9600);
  BTSerial.begin(9600); 
  Serial.println("XE DA SAN SANG! CHO LENH (F, B, L, R, S)...");
}

void loop() {
  if (BTSerial.available()) {
    char command = BTSerial.read(); 
    
    switch (command) {
      case 'F': 
        chayTien(); 
        break;
      case 'B': 
        chayLui(); 
        break;
      case 'L': 
        reTrai(); 
        break;
      case 'R': 
        rePhai(); 
        break;
      case 'S': 
        dungIm(); 
        break;
      default: 
        break; 
    }
  }
}

// --- CÁC HÀM XỬ LÝ CHUYỂN ĐỘNG ---

void chayTien() {
  analogWrite(ENA, tocDo); analogWrite(ENB, tocDo);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);  
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);  
}
void chayLui() {
  analogWrite(ENA, tocDo); analogWrite(ENB, tocDo);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);  
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void reTrai() { 
  analogWrite(ENA, tocDo); analogWrite(ENB, tocDo);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);

}

void rePhai() { 
  analogWrite(ENA, tocDo); analogWrite(ENB, tocDo);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);

}

void dungIm() { 
  analogWrite(ENA, 0); analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}
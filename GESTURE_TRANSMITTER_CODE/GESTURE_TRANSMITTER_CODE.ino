#include <SoftwareSerial.h>

SoftwareSerial BTSerial(2, 3); // Chân RX=2, TX=3 kết nối HC-05

const int xPin = A0; // Trục X của cảm biến GY-61
const int yPin = A1; // Trục Y của cảm biến GY-61

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  Serial.println("TAY CAM DA SAN SANG (F: Tien, B: Lui, L: Trai, R: Phai, S: Dung)...");
}

void loop() {
  int xVal = analogRead(xPin);
  int yVal = analogRead(yPin);
  char command = 'S'; // Mặc định là trạng thái đứng im (Stop)

  // Đọc góc nghiêng để quyết định hướng đi
  // (Lưu ý: Bạn nhớ kiểm tra và căn chỉnh lại 2 con số 280 và 380 cho vừa tay nhé)
  if (xVal < 280) {
    command = 'L'; // Nghiêng tới trước -> Tiến (Forward)
  } 
  else if (xVal > 380) {
    command = 'R'; // Nghiêng ra sau -> Lùi (Backward)
  } 
  else if (yVal < 280) {
    command = 'B'; // Nghiêng trái -> Rẽ trái (Left)
  } 
  else if (yVal > 380) {
    command = 'F'; // Nghiêng phải -> Rẽ phải (Right)
  }

  // Gửi lệnh qua Bluetooth
  BTSerial.print(command);

  // In ra Serial Monitor để debug
  Serial.print("X: "); Serial.print(xVal);
  Serial.print(" | Y: "); Serial.print(yVal);
  Serial.print(" | Lenh gui: "); Serial.println(command);

  delay(100); // Khoảng nghỉ nhỏ tránh nghẽn dữ liệu
}
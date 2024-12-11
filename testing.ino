#include <Wire.h>

// Biến lưu mã lệnh nhận được
volatile uint8_t receivedCommand = 0x00;

void setup() {
  // Khởi tạo Serial để xuất dữ liệu
  Serial.begin(9600);

  // Khởi tạo SMBus (giống I2C) với địa chỉ BMS
  Wire.begin(0x0B); // Địa chỉ SMBus của BMS

  // Cài đặt hàm xử lý khi nhận dữ liệu
  Wire.onReceive(receiveEvent);

  // Cài đặt hàm xử lý khi mainboard yêu cầu dữ liệu
  Wire.onRequest(requestEvent);

  Serial.println("SMBus Slave ready!");
}

void loop() {
  // Không cần xử lý trong vòng lặp chính
}

// Hàm xử lý khi nhận lệnh từ mainboard
void receiveEvent(int bytes) {
  if (bytes > 0) {
    receivedCommand = Wire.read(); // Đọc mã lệnh từ SMBus
    // Ghi log mã lệnh ra Serial Monitor
    Serial.print("Received Command: 0x");
    Serial.println(receivedCommand, HEX);
  }
}

// Hàm xử lý khi mainboard yêu cầu dữ liệu
void requestEvent() {
  // Dữ liệu phản hồi, có thể tuỳ chỉnh theo lệnh nhận được
  uint8_t responseData = 0xFF; // Dữ liệu mặc định
  switch (receivedCommand) {
    case 0x0F: // Ví dụ: Lệnh yêu cầu dung lượng còn lại
      responseData = 50; // Dung lượng giả định 50%
      break;
    default:
      responseData = 0x00; // Lệnh không hợp lệ hoặc chưa xử lý
      break;
  }
  Wire.write(responseData); // Gửi dữ liệu phản hồi
}

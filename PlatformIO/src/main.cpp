#include <Wire.h>

#define SMBUS_ADDRESS 0x08 // Địa chỉ Slave (có thể thay đổi)

void requestEvent()
{
  // Gửi dữ liệu khi nhận yêu cầu từ Master
  Wire.write(0x42); // Ví dụ: gửi một byte dữ liệu
}

void receiveEvent(int howMany)
{
  // Xử lý dữ liệu nhận từ Master
  while (Wire.available()) {
    uint8_t data = Wire.read();
    Serial.print("Received: ");
    Serial.println(data, HEX);
  }
}

void setup() {
  Serial.begin(115200); // Debug qua Serial Monitor
  Wire.begin(SMBUS_ADDRESS); // Cấu hình I2C ở chế độ Slave với địa chỉ SMBus
  Wire.onRequest(requestEvent); // Xử lý khi Master yêu cầu dữ liệu
  Wire.onReceive(receiveEvent); // Xử lý khi nhận dữ liệu từ Master

  Serial.println("SMBus Slave Initialized");
}

void loop() {
  // Không cần xử lý gì trong loop cho giao tiếp SMBus cơ bản
  delay(1000);
}

#include <Arduino.h>

void setup() {
  // Cài đặt chân LED là OUTPUT
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Bật LED
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000); // Đợi 1 giây (1000ms)
  
  // Tắt LED
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000); // Đợi 1 giây (1000ms)
}

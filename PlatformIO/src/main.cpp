#include <Arduino.h>

// Chân đèn LED tích hợp trên Pico (GPIO 25)
#define LED_PIN 25

void setup() {
  // Thiết lập chân LED là OUTPUT
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Bật LED
  digitalWrite(LED_PIN, HIGH);
  delay(500);  // Chờ 500ms

  // Tắt LED
  digitalWrite(LED_PIN, LOW);
  delay(500);  // Chờ 500ms
}

#include <Wire.h>

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Use default SDA (GPIO4) and SCL (GPIO5)

  Serial.println("Scanning I2C devices...");
  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found device at: 0x");
      Serial.println(address, HEX);
    }
  }
}

void loop() {
  // No actions in loop
}

#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.setSDA(1); // GPIO1 for SDA
  Wire.setSCL(2); // GPIO2 for SCL
  Wire.begin();

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

#include <Wire.h>

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Use default SDA (GPIO4) and SCL (GPIO5)

  Serial.println("Starting SMBus scan...");
}

void loop() {
  Serial.println("Scanning SMBus devices...");
  bool deviceFound = false;

  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found SMBus device at: 0x");
      Serial.println(address, HEX);
      deviceFound = true;

      // Example SMBus read: Read a byte from register 0x00
      Wire.beginTransmission(address);
      Wire.write(0x00); // Register address
      if (Wire.endTransmission(false) == 0) { // Send repeated start
        Wire.requestFrom(address, 1); // Request 1 byte
        if (Wire.available()) {
          uint8_t data = Wire.read();
          Serial.print("Data read from device 0x");
          Serial.print(address, HEX);
          Serial.print(": 0x");
          Serial.println(data, HEX);
        }
      }
    }
  }

  if (!deviceFound) {
    Serial.println("No SMBus devices found.");
  }

  delay(1000); // Scan every second
}

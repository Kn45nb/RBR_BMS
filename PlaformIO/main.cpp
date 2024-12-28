#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>

#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5
#define SMBUS_DEVICE_ADDR 0x40 // Địa chỉ thiết bị SMBus

// Khởi tạo I2C
void init_i2c() {
    i2c_init(I2C_PORT, 100 * 1000); // 100 kHz
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    printf("I2C Initialized\n");
}

// Đọc một từ từ SMBus (SMBus Read Word)
uint16_t smbus_read_word(uint8_t address, uint8_t command) {
    uint8_t buffer[2];
    // Gửi lệnh để đọc
    i2c_write_blocking(I2C_PORT, address, &command, 1, true); // Gửi lệnh
    i2c_read_blocking(I2C_PORT, address, buffer, 2, false);  // Đọc 2 byte
    return (buffer[1] << 8) | buffer[0]; // LSB trước, MSB sau
}

// Ghi một byte lên SMBus (SMBus Write Byte)
void smbus_write_byte(uint8_t address, uint8_t command, uint8_t data) {
    uint8_t buffer[2] = {command, data};
    i2c_write_blocking(I2C_PORT, address, buffer, 2, false); // Gửi lệnh và dữ liệu
}

int main() {
    // Thiết lập giao tiếp USB (UART) để xem log
    stdio_init_all();

    // Khởi tạo giao tiếp I2C
    init_i2c();

    while (true) {
        // Đọc dữ liệu từ thiết bị SMBus tại địa chỉ 0x40 với lệnh 0x10
        uint16_t data = smbus_read_word(SMBUS_DEVICE_ADDR, 0x10);
        printf("Data received: 0x%04X\n", data);

        // Gửi một giá trị (ví dụ 0x55) đến thiết bị SMBus
        smbus_write_byte(SMBUS_DEVICE_ADDR, 0x20, 0x55);
        printf("Data 0x55 sent to device.\n");

        sleep_ms(1000); // Đọc mỗi giây
    }
}

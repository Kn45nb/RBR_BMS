#include <Wire.h>

//          Var                         Val             Mô tả                                               Đơn vị      Note
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    #define SMBUS_ADDRESS               0x48          // Địa chỉ SMBus của battery

// Các lệnh SMBus theo chuẩn ACPI 5.0
    #define CMD_BATTERY_STATUS          0x16
    #define CMD_REMAINING_CAPACITY      0x0F
    #define CMD_VOLTAGE                 0x09
    #define CMD_CURRENT                 0x0A
    #define CMD_DESIGN_CAPACITY         0x18
    #define CMD_FULL_CHARGE_CAPACITY    0x10
    #define CMD_CYCLE_COUNT             0x17
    #define CMD_TEMPERATURE             0x08
    #define CMD_RELATIVE_SOC            0x0D
    #define CMD_ABSOLUTE_SOC            0x0E
    #define CMD_AVG_TIME_TO_EMPTY       0x12
    #define CMD_AVG_TIME_TO_FULL        0x13
    #define CMD_CHARGING_CURRENT        0x14
    #define CMD_CHARGING_VOLTAGE        0x15
    #define CMD_MANUFACTURER_NAME       0x20
    #define CMD_DEVICE_NAME             0x21
    #define CMD_SERIAL_NUMBER           0x1C
    #define CMD_ALARM_CAPACITY          0x1A
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//  Type        Var                     Val             Mô tả                                               Đơn vị      Note
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    uint16_t    batteryStatus           = 0x0004;       // Trạng thái pin                                   N/a         Bắt buộc
                                                                                                                    // 0x0001: Sạc
                                                                                                                    // 0x0002: Xả
                                                                                                                    // 0x0004: Full
                                                                                                                    // 0x0008: Low
                                                                                                                    // 0x0010: Error
    uint16_t    remainingCapacity       = 900;          // Dung lượng còn lại của pin                       mAh
    uint16_t    voltage                 = 19000;        // Điện áp hiện tại của pin                         mV
    uint16_t    current                 = 7000;         // Dòng điện pin đang cấp hoặc nhận                 mA          Âm sạc, dương xả
    uint16_t    designCapacity          = 1000;         // Dung lượng thiết kế của pin                      mAh         Không đổi trong vòng đời Cell
    uint16_t    fullChargeCapacity      = 950;          // Dung lượng thực khi sạc đầy                      mAh         
    uint16_t    cycleCount              = 10;           // Số chu kỳ sạc-xả                                 Times
    uint16_t    temperature             = 2980;         // Nhiệt độ hiện tại của pin                        Kelvin
    uint8_t     relativeSOC             = 75;           // Dung lượng còn lại với dung lượng thực           %           =remainingCapacity/fullChargeCapacity to 8bit
    uint8_t     absoluteSOC             = 70;           // Dung lượng còn lại với dung lượng thiết kế       %           =remainingCapacity/designCapacity to 8 bit
    uint16_t    avgTimeToEmpty          = 90;           // Thời gian trung bình cho đến khi pin cạn         Minutes     chỉ áp dụng khi đang dùng   |
    uint16_t    avgTimeToFull           = 120;          // Thời gian trung bình để sạc đầy                  Minutes     Chỉ áp dụng khi đang sạc    |mạch sạc xả cùng chân
    uint16_t    chargingCurrent         = 2000;         // Dòng điện sạc hiện tại                           mA
    uint16_t    chargingVoltage         = 12500;        // Điện áp sạc hiện tại                             mV
    const char  manufacturerName[]      = "Notebook";   // Tên nhà sản xuất pin                             N/a
    const char  deviceName[]            = "BAT";        // Tên thiết bị pin                                 N/a
    const char  serialNumber[]          = "0001";       // Số serial duy nhất của pin                       N/a
    uint16_t    alarmCapacity           = 500;          // Dung lượng ngưỡng cảnh báo pin yếu               mAh         Tùy thuộc vào Cell
    uint8_t     currentCommand          = 0x00;         // Lưu lệnh hiện tại từ Motherboard IMB410TN        N/a

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);               // Set Pin Led

    Serial.begin(9600);                         // Mở Serial

    //lỗi @Kn45nb
    //Wire.setSCL(0);                           // Set Pin Clock
    //Wire.setSDA(1);                           // Set Pin Data
    //Wire.setClock(100000);                    // Set tốc

    Wire.begin(1, 2);                           // Mở cổng SMBus
    Serial.println("0x00");
}

void loop()
{
    Serial.println("0x0000");   

    if (true /* Testing */)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
    }
    else
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
    }




    
    delay(1000);
}

// Hàm xử lý khi nhận lệnh SMBus từ motherboard
void receiveEvent(int numBytes)
{
    if (numBytes < 1) return;                   // Không nhận được lệnh nào
    currentCommand = Wire.read();               // Đọc lệnh từ master
}

// Hàm xử lý khi motherboard yêu cầu dữ liệu
void requestEvent()
{
    switch (currentCommand)
    {
        case CMD_BATTERY_STATUS:
            send16BitData(batteryStatus);
            break;
        case CMD_REMAINING_CAPACITY:
            send16BitData(remainingCapacity);
            break;
        case CMD_VOLTAGE:
            send16BitData(voltage);
            break;
        case CMD_CURRENT:
            send16BitData(current);
            break;
        case CMD_DESIGN_CAPACITY:
            send16BitData(designCapacity);
            break;
        case CMD_FULL_CHARGE_CAPACITY:
            send16BitData(fullChargeCapacity);
            break;
        case CMD_CYCLE_COUNT:
            send16BitData(cycleCount);
            break;
        case CMD_TEMPERATURE:
            send16BitData(temperature);
            break;
        case CMD_RELATIVE_SOC:
            send8BitData(relativeSOC);
            break;
        case CMD_ABSOLUTE_SOC:
            send8BitData(absoluteSOC);
            break;
        case CMD_AVG_TIME_TO_EMPTY:
            send16BitData(avgTimeToEmpty);
            break;
        case CMD_AVG_TIME_TO_FULL:
            send16BitData(avgTimeToFull);
            break;
        case CMD_CHARGING_CURRENT:
            send16BitData(chargingCurrent);
            break;
        case CMD_CHARGING_VOLTAGE:
            send16BitData(chargingVoltage);
            break;
        case CMD_MANUFACTURER_NAME:
            sendStringData(manufacturerName);
            break;
        case CMD_DEVICE_NAME:
            sendStringData(deviceName);
            break;
        case CMD_SERIAL_NUMBER:
            sendStringData(serialNumber);
            break;
        case CMD_ALARM_CAPACITY:
            send16BitData(alarmCapacity);
            break;
        default:
            Wire.write(0x00);                   // None
            break;
    }
}

// Hàm gửi dữ liệu 16-bit qua SMBus
void send16BitData(uint16_t data)
{
    Wire.write(data & 0xFF);                    // Byte thấp
    Wire.write((data >> 8) & 0xFF);             // Byte cao
}

// Hàm gửi dữ liệu 8-bit qua SMBus
void send8BitData(uint8_t data)
{
    Wire.write(data);
}

// Hàm gửi chuỗi ASCII qua SMBus    ( Giới hạn chuỗi tối đa 32 ký tự )
void sendStringData(const char* data)
{
    uint8_t count = 0;
    while (*data && count < 32)
    {
        Wire.write(*data++);
        count++;
    }
}

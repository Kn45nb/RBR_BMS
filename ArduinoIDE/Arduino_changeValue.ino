/*
  Arduino_changeValue
  
  */
 
 #include <Wire.h>

// The Following variables are write only by the Serial Interface 
// and read Only for the I2C Master all other Values for the Smart battery are constants

unsigned int Temperature            = 2980;    // 0 - 65535 * 0.1K   initial = 2980= 298K = ca. 25°C
unsigned int Voltage                = 11000;   // 0 - 65535 mV       initial = 11V = 11000mV
  signed int Current                = -300;    // -32768 - 0mA       initial = -0,3A
unsigned int RelativeStateOfCharge  = 100;     // 0 - 100%           initial = 100%
unsigned int AbsoluteStateOfCharge  = 100;     // 0 - 100+%          initial = 100%
unsigned int RemainingCapacity      = 2700;     // 0 - 65535 mAh      initial = 
unsigned int BatteryStatus          = 0x00C0;   // 0xC = B1100: bit 4 = Initialized, bit 3 = Discharging
unsigned int RunTimeToEmpty         = 700;     // 0 - 65534 min      initial = 700min = 11,6h
unsigned int AverageTimeToEmpty     = 700;     // 0 - 65534 min      initial = 700min = 11,6h


// The following constants are Smart Battery Values which are constant but still be requested

//Varaible/constant                                    Output       Address
const unsigned int ManufactureAccess      = 0x010A;  // word         0x00
const unsigned int RemainingCapacityAlarm = 0x0000; // unsigned int 0x01
const unsigned int RemainingTimeToAlarm   = 0x0000; // unsigned int 0x02
const unsigned int BatteryMode            = 0x6001; // unsigned int 0x03
const signed int   AtRate                 = 0x0000; // signed int   0x04
const unsigned int AtRateTimeToFull       = 0xFFFF; // unsigned int 0x05
const unsigned int AtRateTimeToEmpty      = 0xFFFF; // unsigned int 0x06
const boolean      AtRateOK               = 0x0001; // boolean      0x07

const signed int   AverageCurrent         = -268;   // signed int   0x0b
const unsigned int MaxError               = 0x0001;  // unsigned int 0x0c
const unsigned int FullChargeCapacity     = 3265;  // unsigned int 0x10


const unsigned int AverageTimeToFull      = 0xFFFF; // unsigned int 0x13
const unsigned int CycleCount             = 0x00C0; // unsigned int 0x17
const unsigned int DesignCapacity         = 3350;   // unsigned int 0x18
const unsigned int DesignVoltage          = 10800;  // unsigned int 0x19
const unsigned int SpecificationInfo      = 0x31;   // unsigned int 0x1a
const unsigned int ManufactureDate        = 0x42C5; // 05 june 2013 (0-4day,5-8mon,9-15 year+1980) unsigned int 0x1b 
const unsigned int SerialNumber           = 0x0002; // unsigned int 0x1c
const char         ManufacturerName[]     = "DREAGER";             // string       0x20
const char         DeviceName[]           = "IP1-3S1p-000000022";  // string       0x21
const char         DeviceChemistry[]      = "LI0N";                // string       0x22
const char         ManufacturerData[]     = "0x0E0001000100010001000C000B0000";      // block        0x23

//more constants
const unsigned int ChargingCurrent        = 0x0658;  // const unsigned int 0x14
const unsigned int ChargingVoltage        = 0x3138;  // const unsigned int 0x15

const char         OptionalFunction[]       = "0x140000000000000000000000000000000000000000";

const unsigned int VoltageCell1           = 0;      // unsigned int 0x3c
const unsigned int VoltageCell2           = 3500;   // unsigned int 0x3d
const unsigned int VoltageCell3           = 3500;   // unsigned int 0x3e
const unsigned int VoltageCell4           = 3500;   // unsigned int 0x3f

const char         ErrorMessage[]         = "something";
// The Programm starts here


char adress = ' ';                // char used to switch to the right adress
String value = "";                // a string to hold incoming data
boolean stringComplete = false;   // whether the string is complete
boolean request = false;          // true if a I²C request was sent
int command = -1;

void setup() {
  Wire.begin(11);             // join i2c bus with address #16 or 17?
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  // initialize serial:
  Serial.begin(9600);
  // reserve 20 bytes for the inputString:
  value.reserve(20);
}

void loop() {  
  // change the value of the set variable when a newline arrives
  if (stringComplete) {
    switch(adress) {
       case 'a': Temperature = value.toInt(); break;
       case 'b': Voltage = value.toInt(); break;
       case 'c': Current = value.toInt(); break;
       case 'd': RelativeStateOfCharge = value.toInt(); break;
       case 'e': AbsoluteStateOfCharge = value.toInt(); break;
       case 'f': RemainingCapacity = value.toInt(); break;
       case 'g': BatteryStatus = value.toInt(); break;
       case 'h': RunTimeToEmpty = value.toInt(); break;
       case 'i': AverageTimeToEmpty = value.toInt(); break;
       default: break;
    }
    // clear the variables:
    adress = ' ';
    value = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // determine wether the adress was set or not
    if (adress == ' ') {
      // set the adress to the sent char
      adress = inChar;
    } else {
      // add it to the inputString:
      value += inChar;
    }
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int numOfBytes)
{
    command = Wire.read();    // receive byte as an integer 
}

void requestEvent()
{
    switch(command){
      case 0:
        Wire.write(lowByte(ManufactureAccess)); 
        Wire.write(highByte(ManufactureAccess));
        break;
      case 1: Wire.write(RemainingCapacityAlarm); break;
      case 2: Wire.write(RemainingTimeToAlarm); break;
      case 3: Wire.write(BatteryMode); break;
      case 4: Wire.write(AtRate); break;
      case 5: Wire.write(AtRateTimeToFull); break;
      case 6: Wire.write(AtRateTimeToEmpty); break;
      case 7: Wire.write(AtRateOK); break;
      case 8: Wire.write(Temperature); break;
      case 9: Wire.write(Voltage); break;
      case 0x0a: Wire.write(Current); break;
      case 0x0b: Wire.write(AverageCurrent); break;
      case 0x0c: Wire.write(MaxError); break;
      case 0x0d: Wire.write(RelativeStateOfCharge); break;
      case 0x0e: Wire.write(AbsoluteStateOfCharge); break;
      case 0x0f: Wire.write(RemainingCapacity); break;   
      case 0x10: Wire.write(FullChargeCapacity); break;
      case 0x11: Wire.write(RunTimeToEmpty); break;
      case 0x12: Wire.write(AverageTimeToEmpty); break;
      case 0x13: Wire.write(AverageTimeToFull); break;
      case 0x14: Wire.write(ChargingCurrent); break;
      case 0x15: Wire.write(ChargingVoltage); break;
      case 0x16: Wire.write(BatteryStatus); break;
      case 0x17: Wire.write(CycleCount); break;
      case 0x18: Wire.write(DesignCapacity); break;
      case 0x19: Wire.write(DesignVoltage); break;
      case 0x1a: Wire.write(SpecificationInfo); break;
      case 0x1b: Wire.write(ManufactureDate); break;
      case 0x1c: Wire.write(SerialNumber); break;
      case 0x1d: Wire.write("TBD"); break;
      case 0x1e: Wire.write("TBD"); break;
      case 0x1f: Wire.write("TBD"); break;     
      case 0x20: Wire.write(ManufacturerName); break;
      case 0x21: Wire.write(DeviceName); break;
      case 0x22: Wire.write(DeviceChemistry); break;
      case 0x23: Wire.write(ManufacturerData); break;   
      case 0x3c: Wire.write(VoltageCell1); break;
      case 0x3d: Wire.write(VoltageCell2); break;
      case 0x3e: Wire.write(VoltageCell3); break;
      case 0x3f: Wire.write(VoltageCell4); break;    
      //default: Wire.write(ErrorMessage); break;
    }
}


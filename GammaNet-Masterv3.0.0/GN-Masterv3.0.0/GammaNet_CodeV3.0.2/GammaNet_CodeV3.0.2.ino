#include "core1_tasks.h"  // Data Collection
#include "settings.h"  // Node Settings
#include "pinSetup.h"  // PinSetup - Pin Settings

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for microseconds to seconds */
#define SLEEP_DURATION 300     /* 300 Sleep time (10 minutes in seconds) */
RTC_DATA_ATTR int bootCount = 0;
unsigned long startMillis = millis();
HardwareSerial GPS_Serial(2);  // UART2 for GPS
// Define RX and TX pins for the GPS
static const int RXPin = 33, TXPin = 32;
static const uint32_t GPSBaud = 9600; // GT-U7 default baud rate
byte ubxCfgNav[] = { 
  0xB5, 0x62, 0x06, 0x3E, 0x14, 0x00, 
  0x00, 0x00, 0x20, 0x00, 0x00, 0x08, 0x08, 0x00, 
  0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x01, 0x00, 0x6B, 0x60 
};



void sendUBXCommand(byte *cmd, uint16_t len) {
  for (uint16_t i = 0; i < len; i++) {
    GPS_Serial.write(cmd[i]);
  }
}

void setup() {
    Serial.begin(115200);
    GPS_Serial.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);
     Serial.println("Sending GNSS Config Command...");
  sendUBXCommand(ubxCfgNav, sizeof(ubxCfgNav));
    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));
pinMode(Relay3v, OUTPUT);
    // Start Core 1 tasks (Data Collection)
    xTaskCreatePinnedToCore(
        core1Task, "Core1Task", 2048, NULL, 1, NULL, 1);
}

void loop() {
}

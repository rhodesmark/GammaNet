#include <Arduino.h>
#include "esp_sleep.h"
#include "core1_tasks.h"
#include "nodeSettings.h"
#include "systemSettings.h"
#include "pinSetUp.h"  // Pin Setup and Pin Defnines

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
    delay(1000);
      //GPS BaudRate and Pins
  GPS_Serial.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);
  //Setting the GPS Module Up for GNSS Commands
  Serial.println("Sending GNSS Config Command...");
  sendUBXCommand(ubxCfgNav, sizeof(ubxCfgNav));
    // Start Core 1 task
    xTaskCreatePinnedToCore(core1Task, "Core1 Task", 10000, NULL, 1, NULL, 1);

      //Set Up Pin for 3v Relay as an Output
  pinMode(Relay3v, OUTPUT);
}

void loop() {
    // Core 0 tasks here
}
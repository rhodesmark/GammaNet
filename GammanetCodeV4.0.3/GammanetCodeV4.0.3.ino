#include <Arduino.h>
#include "esp_sleep.h"
#include "core1_tasks.h"
#include "nodeSettings.h"
#include "systemSettings.h"
#include "pinSetUp.h"  // Pin Setup and Pin Defnines
#include "FS.h"
#include <SD.h>
#include <SPI.h>

#define SD_CS 5
#define SCK_PIN 18
#define MISO_PIN 19
#define MOSI_PIN 23
HardwareSerial GPS_Serial(2);  // UART2 for GPS
SPIClass spiSD(HSPI);


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
    //Setup MicroSD Card Module and Read Card
if(DeviceType == "ESP32-32 - LilyGo A7608SA-H"){
    SPI.begin(BOARD_SCK_PIN, BOARD_MISO_PIN, BOARD_MOSI_PIN);
    while (!SD.begin(BOARD_SD_CS_PIN)) {
    Serial.println("No SD card detected. Insert card...");
    delay(1000);  // Wait 1 second before checking again
    Serial.println("SD card detected! Proceeding...");
    }
    uint8_t cardType = SD.cardType();
      if (cardType == CARD_NONE) {
      Serial.println("No SD card attached");
      return;
      }
}

else if(DeviceType == "ESP32-Wroom-32"){
  Serial.println("Initializing SD card...");
  delay(1000);
   SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SD_CS);  // Set custom SPI pins

  if (!SD.begin(SD_CS)) {
    Serial.println("SD card initialization failed!");
    while (true);  // Stop execution
  }
  Serial.println("SD card initialized.");
}//End of SD card Set up for: ESP32-Wroom-32

else if(DeviceType == "ESP32-Wroom-32_38"){}
else{
    Serial.print(F("No Node Type Defined."));
}
//End of Setup MicroSD Card Module and Read Card

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

void powerOnTone(){}
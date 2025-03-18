#include "core0_tasks.h"
#include "core1_tasks.h"  // Data Collection
#include "MenuSystem.h"
#include "nodeSettings.h"  // Node Settings
#include "systemSettings.h"  // System Settings Page
#include "pinSetup.h"  // PinSetup - Pin Settings
#include "variables.h"  // Node Settings
#include <SD.h>
#include "SPI.h"


#define uS_TO_S_FACTOR 1000000  /* Conversion factor for microseconds to seconds */
#define SLEEP_DURATION 300     /* 300 Sleep time (10 minutes in seconds) */
RTC_DATA_ATTR int bootCount = 0;
unsigned long startMillis = millis();
#include <HardwareSerial.h>

// Create Serial objects for each core
//HardwareSerial Serial1(1); // Core 0
//HardwareSerial Serial2(2); // Core 1

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
  //Micro Sd Card Module Setup
  delay(2000);
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

    if (!SD.begin(SD_CS)) {
        Serial.println("Card Mount Failed!");
        return;
    }

   uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        Serial.println("No SD card detected!");
        return;
    }

    // Optionally, check the card's type
    if (cardType == CARD_MMC) {
        Serial.println("MMC card detected.");
    } else if (cardType == CARD_SD) {
        Serial.println("SD card detected.");
    } else if (cardType == CARD_SDHC) {
        Serial.println("SDHC card detected.");
    } else {
        Serial.println("Unknown card type.");
    }

  //GPS BaudRate and Pins
  GPS_Serial.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);
  //Setting the GPS Module Up for GNSS Commands
  Serial.println("Sending GNSS Config Command...");
  sendUBXCommand(ubxCfgNav, sizeof(ubxCfgNav));
  //Boot/Wake Number
   ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

 if(NodeType == "Central Hub"){
  pinMode(SolarInVolts, INPUT); // Set the voltage pin as input
 } else if(NodeType == "Node Centre"){

 } else if(NodeType == "Sensor Node"){

 } else if(NodeType == "Scatter Node Internal"){

 } else if(NodeType == "Scatter Node External"){

 } else if(NodeType == "Repeater Node"){

 } else {
  Serial.println(F("No Node Type Defined - Pins Cannnot be defined."));
 }

  //Set Up Pin for 3v Relay as an Output
  pinMode(Relay3v, OUTPUT);

displayMenu();

Serial.println(F("Allow Core 1 Tasks to Run"));
xTaskCreatePinnedToCore(
        core1Task, 
        "Core1Task",
        2048, 
        NULL, 
        1, 
        NULL, 
        1
    );

}

void loop() {
 handleMenu();
}

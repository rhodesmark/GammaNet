#ifndef GPS_HANDLER_H
#define GPS_HANDLER_H

#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

// GPS RX/TX pins (ESP32)
#define GPS_RX 26   // GPS TX -> ESP32 GPIO 26
#define GPS_TX 27   // GPS RX -> ESP32 GPIO 27

extern TinyGPSPlus gps;
extern HardwareSerial gpsSerial;

// Function prototypes
void initGPS();
void readGPS();
String getGPSData();

#endif

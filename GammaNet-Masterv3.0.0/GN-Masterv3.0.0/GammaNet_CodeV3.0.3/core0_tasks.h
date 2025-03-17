#ifndef CORE0_TASKS_H
#define CORE0_TASKS_H

#include <Arduino.h>
#include <WiFi.h>

// AP Settings (Modify as needed)
extern const char* ap_ssid;
extern const char* ap_password;

// Function Declarations
void startWiFiAPTask();
void WiFiAPTask(void* parameter);

#endif


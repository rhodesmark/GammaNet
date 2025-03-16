#include "core1_tasks.h"
#include <Arduino.h>
#include <TinyGPS++.h>
#include "settings.h"  // Node Settings
#include "pinSetUp.h"  // Node Settings

extern HardwareSerial GPS_Serial;
TinyGPSPlus gps;


// Function Prototypes (Add this at the top of the file)

void core1Task(void *pvParameters) {
    int gpsHour, gpsMinute, gpsSecond, gpsCentisecond;

    Serial.println("Core 1: Getting GPS time...");
    
    if (!getGPSTime(gpsHour, gpsMinute, gpsSecond, gpsCentisecond)) {
        Serial.println("Core 1: GPS time unavailable, defaulting to sleep.");
        goToSleep(300);  // Default to 5 minutes sleep if GPS time is unavailable
    }

    Serial.printf("GPS Time: %02d:%02d:%02d.%02d\n", gpsHour, gpsMinute, gpsSecond, gpsCentisecond);

    // Check if sleepTest is "Yes"
    if (sleepTest == "Yes") {
        // Sleep for 5 minutes (300 seconds)
        Serial.println("Sleep Test: Sleeping for 5 minutes.");
        goToSleep(300);
    } else {
        // Corrected function call: only pass gpsMinute
        if (isWakePeriod(gpsMinute)) {
            int centisecondsUntilNextSleep = ((5 - (gpsMinute % 15)) * 60 - gpsSecond) * 100 + (100 - gpsCentisecond);
            Serial.printf("Core 1: Staying awake for %.2f seconds\n", centisecondsUntilNextSleep / 100.0);
            stayAwake(centisecondsUntilNextSleep);
        } else {
            int secondsUntilNextWake = calculateSleepDuration(gpsMinute, gpsSecond);
            Serial.printf("Core 1: Sleeping for %d seconds\n", secondsUntilNextWake);
            goToSleep(secondsUntilNextWake);
        }
    }
}

// Function Definitions
bool isWakePeriod(int minute) {
    return (minute >= 5 && minute < 10) || 
           (minute >= 15 && minute < 20) || 
           (minute >= 30 && minute < 35) || 
           (minute >= 45 && minute < 50);
}

int calculateSleepDuration(int minute, int second) {
    int nextWakeMinute;
    
    if (minute < 5)       nextWakeMinute = 5;
    else if (minute < 15) nextWakeMinute = 15;
    else if (minute < 30) nextWakeMinute = 30;
    else if (minute < 45) nextWakeMinute = 45;
    else { 
        nextWakeMinute = 5;  // Next wake time is in the next hour
    }

    int sleepSeconds = ((nextWakeMinute - minute) * 60) - second;
    if (sleepSeconds < 0) sleepSeconds += 3600;  // Ensure valid sleep time

    return sleepSeconds;
}

bool getGPSTime(int &hour, int &minute, int &second, int &centisecond) {
    if (gps.location.isUpdated()) {
        if (gps.time.isValid()) {
            hour = gps.time.hour();
            minute = gps.time.minute();
            second = gps.time.second();
            centisecond = gps.time.centisecond();
            return true;  // Successfully retrieved time
        }
    }
    Serial.println("ERROR: GPS time is invalid.");
    return false;  // If GPS time is not valid
}

void stayAwake(int awakeCentiseconds) {
    Serial.print(F("Waking Up"));
    delay(1000);
    
    // Turn off 3v Relay Switch
    if(debugMode == "On"){
        Serial.println(F("3v Relay On"));
        digitalWrite(Relay3v, HIGH); // Turns relay ON
        delay(50);
    } else {
        digitalWrite(Relay3v, HIGH); // Turns relay ON
    }

    delay(1000);
    unsigned long startMillis = millis();
    while ((millis() - startMillis) < (awakeCentiseconds * 10)) {
        // Add If Else Case here depending on Node Type
        if(NodeType == "Central Hub"){
            Serial.println(F("Central Hub Programme Running"));
            Serial.println(F("Core 1: Collecting GPS data..."));
            while (GPS_Serial.available() > 0) {
                if (gps.encode(GPS_Serial.read())) {
                    if(debugMode == "On"){
                        displayNodeInfo(); //Display Node Information
                        displayGPSInfo(); //Display GPS Information
                        CentralHubProgramme(); // Central Hub Main Programme
                    } else {
                        getGPSInfo(); // Get GPS Info - Do NOT Print
                    }
                }
            }
            // Add Sensor Data Here
            delay(1000);
        }
        else if(NodeType == "Node Centre"){
            Serial.println(F("Node Centre Programme Running"));
            Serial.println(F("Core 1: Collecting GPS data..."));
            while (GPS_Serial.available() > 0) {
                if (gps.encode(GPS_Serial.read())) {
                    if(debugMode == "On"){
                        displayNodeInfo(); // Display Node Information
                        displayGPSInfo(); // Display GPS Information
                        NodeCentreProgramme(); // Node Centre Programme
                    } else {
                        getGPSInfo(); // Get GPS Info - Do NOT Print
                    }
                }
            }
            // Add Sensor Data Here
            delay(1000);
        }
        else if(NodeType == "Sensor Node"){
            Serial.println(F("Sensor Node Programme Running"));
            Serial.println(F("Core 1: Collecting GPS data..."));
            while (GPS_Serial.available() > 0) {
                if (gps.encode(GPS_Serial.read())) {
                    if(debugMode == "On"){
                        displayNodeInfo(); // Display Node Information
                        displayGPSInfo(); // Display GPS Information
                        SensorNodeProgramme(); // Sensor Node Programme
                    } else {
                        getGPSInfo(); // Get GPS Info - Do NOT Print
                    }
                }
            }
            // Add Sensor Data Here
            delay(1000);
        }
        else if(NodeType == "Scatter Node External"){
            Serial.println(F("Scatter Node (External) Programme Running"));
            Serial.println(F("Core 1: Collecting GPS data..."));
            while (GPS_Serial.available() > 0) {
                if (gps.encode(GPS_Serial.read())) {
                    if(debugMode == "On"){
                        displayNodeInfo(); // Display Node Information
                        displayGPSInfo(); // Display GPS Information
                        ScatterNodeExtProgramme(); // Scatter Node External Programme
                    } else {
                        getGPSInfo(); // Get GPS Info - Do NOT Print
                    }
                }
            }
            // Add Sensor Data Here
            delay(1000);
        }
        else if(NodeType == "Scatter Node Internal"){
            Serial.println(F("Scatter Node (Internal) Programme Running"));
            Serial.println(F("Core 1: Collecting GPS data..."));
            while (GPS_Serial.available() > 0) {
                if (gps.encode(GPS_Serial.read())) {
                    if(debugMode == "On"){
                        displayNodeInfo(); // Display Node Information
                        displayGPSInfo(); // Display GPS Information
                        ScatterNodeIntProgramme(); // Scatter Node Internal Programme
                    } else {
                        getGPSInfo(); // Get GPS Info - Do NOT Print
                    }
                }
            }
            // Add Sensor Data Here
            delay(1000);
        }
        else if(NodeType == "Repeater Node"){
            Serial.println(F("Repeater Node Programme Running"));
            Serial.println(F("Core 1: Collecting GPS data..."));
            while (GPS_Serial.available() > 0) {
                if (gps.encode(GPS_Serial.read())) {
                    if(debugMode == "On"){
                        displayNodeInfo(); // Display Node Information
                        displayGPSInfo(); // Display GPS Information
                        RepeaterNodeProgramme(); // Repeater Node Programme
                    } else {
                        getGPSInfo(); // Get GPS Info - Do NOT Print
                    }
                }
            }
            // Add Sensor Data Here
            delay(1000);
        }
        else {
            Serial.println(F("ERROR: -- No Node Type Selected --"));
            Serial.println(F("Core 1: Collecting GPS data..."));
            while (GPS_Serial.available() > 0) {
                if (gps.encode(GPS_Serial.read())) {
                    if(debugMode == "On"){
                        displayNodeInfo(); // Display Node Information
                        displayGPSInfo(); // Display GPS Information
                    } else {
                        getGPSInfo(); // Get GPS Info - Do NOT Print
                    }
                }
            }
            // Add Sensor Data Here
            delay(1000);
        }
    }
    Serial.println("Core 1: Finished data collection, going to sleep...");
    delay(100);
    goToSleep(600);
}

void goToSleep(int sleepSeconds) {
    // Turn off 3v Relay Switch
    if(debugMode == "On"){
        Serial.println(F("3v Relay Off"));
        digitalWrite(Relay3v, LOW); // Turns relay OFF
    } else {
        digitalWrite(Relay3v, LOW); // Turns relay OFF
    }

    Serial.printf("Core 1: ESP32 going to sleep for %d seconds...\n", sleepSeconds);
    delay(1000);
    esp_sleep_enable_timer_wakeup(sleepSeconds * 1000000);
    Serial.flush();
    esp_deep_sleep_start();
}

void displayGPSInfo() {
    Serial.println(F("Location Data from GT-U7 GPS Module."));
    if (gps.date.isValid()) {
        Serial.print(gps.date.month());
        Serial.print(F("/"));
        Serial.print(gps.date.day());
        Serial.print(F("/"));
        Serial.print(gps.date.year());
        Serial.print(F(" | "));
    } else {
        Serial.print(F("ERROR: GPS DATE INVALID"));
    }
    if (gps.time.isValid()) {
        if (gps.time.hour() < 10) Serial.print(F("0"));
        Serial.print(gps.time.hour());
        Serial.print(F(":"));
        if (gps.time.minute() < 10) Serial.print(F("0"));
        Serial.print(gps.time.minute());
        Serial.print(F(":"));
        if (gps.time.second() < 10) Serial.print(F("0"));
        Serial.print(gps.time.second());
        Serial.print(F("."));
        if (gps.time.centisecond() < 10) Serial.print(F("0"));
        Serial.print(gps.time.centisecond());
        Serial.print(F(" | "));
    } else {
        Serial.print(F("ERROR: GPS TIME INVALID"));
    } 
    if (gps.location.isValid()) {
        Serial.print(gps.location.lat(), 6);
        Serial.print(F(" | "));
        Serial.print(gps.location.lng(), 6);
        Serial.print(F(" | "));
    } else {
        Serial.print(F("ERROR: GPS LOCATION DATA INVALID"));
    }
    if (gps.satellites.isValid()) {
        Serial.print(gps.satellites.value());
    } else {
        Serial.print(F("ERROR: GPS SATELLITES COUNT INVALID"));
        Serial.print(F(" | "));
    }
    Serial.println();
}

void getGPSInfo() {
    if (gps.date.isValid()) {
        gps.date.month();
        gps.date.day();
        gps.date.year();
    } else {
        Serial.println(F("ERROR: GPS DATE INVALID"));
    }
    if (gps.time.isValid()) {
        gps.time.hour();
        gps.time.minute();
        gps.time.second();
        gps.time.centisecond();
    } else {
        Serial.println(F("ERROR: GPS TIME INVALID"));
    }
    if (gps.location.isValid()) {
        gps.location.lat();
        gps.location.lng();
    } else {
        Serial.println(F("ERROR: GPS LOCATION DATA INVALID"));
    }
}


void displayNodeInfo(){
  Serial.print(SystemName);
  Serial.print(F(" | "));
  Serial.print(DeviceType);
  Serial.print(F(" | "));
  Serial.print(NodeType);
  Serial.print(F(" | "));
  Serial.print(NodeStyle);
  Serial.print(F(" | "));
  Serial.println(NodeID);
}

void getNodeInfo(){
  SystemName;
  DeviceType;
  NodeType;
  NodeStyle;
  NodeID;
}

void CentralHubProgramme(){
//

}

void NodeCentreProgramme(){
  //
}

void SensorNodeProgramme(){
  //
}

void ScatterNodeExtProgramme(){
  //
}

void ScatterNodeIntProgramme(){
  //
}

void RepeaterNodeProgramme(){
  //
}

void CH2Web(){}


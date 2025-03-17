#include "core1_tasks.h"
#include <Arduino.h>
#include <TinyGPS++.h>
#include "settings.h"  // Node Settings
#include "pinSetUp.h"  // Node Settings

extern HardwareSerial GPS_Serial;
TinyGPSPlus gps;


int gpsCount = 0;  // Counter for the number of processed results
// Function Prototypes (Add this at the top of the file)

void core1Task(void *pvParameters) {
    int gpsHour, gpsMinute, gpsSecond, gpsCentisecond;
    unsigned long startMillis = millis();  // Start time for the 1-minute collection period

    // Allow GPS to run for 25 seconds for testing (change to 60000 for live)
    Serial.println("Core 1: Starting GPS Time collection for 25 seconds...");

    while (millis() - startMillis < 25000) {
        if (GPS_Serial.available()) {
            while (GPS_Serial.available()) {
                gps.encode(GPS_Serial.read()); // Decode incoming GPS data
            }

            if (gps.time.isUpdated()) {
                gpsCount++;  // Increment count when GPS time is updated
                
                if (debugMode == "On") {
                    Serial.println(F("CHECKING WAKING GPS TIME DEBUG MODE ON"));
                    displayNodeInfo();
                    displayGPSInfo();
                } else {
                    getGPSInfo();
                }
            }
        }
        delay(1000);
    }

    Serial.println(F("Core 1: Checking waking GPS time..."));

    if (gps.time.isValid()) {
        gpsHour = gps.time.hour();
        gpsMinute = gps.time.minute();
        gpsSecond = gps.time.second();
        gpsCentisecond = gps.time.centisecond();

        if (gpsHour >= 0 && gpsHour < 24 && gpsMinute >= 0 && gpsMinute < 60 && gpsSecond >= 0 && gpsSecond < 60) {
            Serial.printf("GPS Time: %02d:%02d:%02d.%02d\n", gpsHour, gpsMinute, gpsSecond, gpsCentisecond);
        } else {
            Serial.println("ERROR: GPS Waking time is invalid.");
            goToSleep(300);
            return;
        }
    } else {
        Serial.println("ERROR: GPS Waking time is unavailable.");
        goToSleep(300);
        return;
    }

    int nextWakeSecond = 0;
    int nextWakeMinute = -1;

    if (sleepTest == "Yes") {
        if (gpsMinute < 5) nextWakeMinute = 10;
        else if (gpsMinute < 15) nextWakeMinute = 15;
        else if (gpsMinute < 25) nextWakeMinute = 25;
        else if (gpsMinute < 35) nextWakeMinute = 35;
        else if (gpsMinute < 45) nextWakeMinute = 45;
        else if (gpsMinute < 55) nextWakeMinute = 55;
        else nextWakeMinute = 0;
    } else {
        if (gpsMinute < 5) nextWakeMinute = 15;
        else if (gpsMinute < 20) nextWakeMinute = 20;
        else if (gpsMinute < 35) nextWakeMinute = 35;
        else if (gpsMinute < 50) nextWakeMinute = 50;
        else nextWakeMinute = 0;
    }

    Serial.printf("Fixed Next wake minute: %d\n", nextWakeMinute);

    int timeUntilNextWake = ((nextWakeMinute - gpsMinute) * 60) - gpsSecond;

    if (timeUntilNextWake < 0) {
        Serial.println("Next wake time is in the next hour.");
        timeUntilNextWake += 3600;
    }

    Serial.printf("Fixed time until next wake: %d seconds\n", timeUntilNextWake);

    if (timeUntilNextWake <= 0) {
        timeUntilNextWake = 300;
        Serial.println("Core 1: Defaulting to 5 minutes sleep due to invalid next wake time.");
    }

    // Check if the ESP32 is currently in a wake period
    bool isWithinWakePeriod = false;

    if (sleepTest == "Yes") {
        isWithinWakePeriod = (gpsMinute >= 0 && gpsMinute < 5) || 
                             (gpsMinute >= 10 && gpsMinute < 15) || 
                             (gpsMinute >= 20 && gpsMinute < 25) || 
                             (gpsMinute >= 30 && gpsMinute < 35) || 
                             (gpsMinute >= 40 && gpsMinute < 45) || 
                             (gpsMinute >= 50 && gpsMinute < 55);
    } else {
        isWithinWakePeriod = (gpsMinute >= 0 && gpsMinute < 5) || 
                             (gpsMinute >= 15 && gpsMinute < 20) || 
                             (gpsMinute >= 30 && gpsMinute < 35) || 
                             (gpsMinute >= 45 && gpsMinute < 50);
    }

    if (isWithinWakePeriod) {
        int remainingTimeInWakePeriod = ((nextWakeMinute - gpsMinute) * 60) - gpsSecond;

        if (remainingTimeInWakePeriod > 0) {
            Serial.printf("Core 1: Staying awake for %d seconds until next wake period at %02d:%02d\n", 
                          remainingTimeInWakePeriod, gpsHour, nextWakeMinute);
            stayAwake(remainingTimeInWakePeriod);
            return;
        } else {
            Serial.println("Core 1: Already in a wake period, no need to sleep.");
            return;
        }
    }

    Serial.printf("Core 1: Sleeping for %d seconds until next wake period at %02d:%02d\n", 
                  timeUntilNextWake, gpsHour, nextWakeMinute);
    goToSleep(timeUntilNextWake);
}


bool getGPSTime(int &hour, int &minute, int &second, int &centisecond) {
    if (gps.location.isUpdated()) {
        if (gps.time.isValid()) {
            hour = gps.time.hour();
            minute = gps.time.minute();
            second = gps.time.second();
            centisecond = gps.time.centisecond();
            delay(10000);
            return true;  // Successfully retrieved time
        }
    }
    Serial.println("ERROR: GPS time is invalid.");
    return false;  // If GPS time is not valid
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

    // Determine the next wake time (it can be 5, 15, 30, or 45 minutes)
    if (minute < 5)       nextWakeMinute = 15;
    else if (minute < 15) nextWakeMinute = 15;
    else if (minute < 30) nextWakeMinute = 30;
    else if (minute < 45) nextWakeMinute = 45;
    else if (minute < 60) nextWakeMinute = 5;  // Next wake is in the next hour

    // Calculate the time until next wake (in seconds)
    int sleepSeconds = ((nextWakeMinute - minute) * 60) - second;

    // If sleep seconds is negative, we need to add the number of seconds in an hour
    if (sleepSeconds < 0) {
        sleepSeconds += 3600;  // Add one hour
    }

    Serial.printf("Minute: %d, Next Wake Minute: %d, Sleep Duration: %d seconds\n", minute, nextWakeMinute, sleepSeconds);

    return sleepSeconds;
}


void stayAwake(int durationSeconds) {
  Serial.printf("Staying awake for %d seconds\n", durationSeconds);
    Serial.print(F("Waking Up"));
    delay(1000);
    
    // Turn off 3v Relay Switch
    if(debugMode == "On"){
        Serial.println(F("3v Relay Module Switiching DEBUG MODE ON"));
        Serial.println(F("3v Relay On"));
        digitalWrite(Relay3v, HIGH); // Turns relay ON
        delay(50);
    } else {
      //3v Relay Switching On.
        digitalWrite(Relay3v, HIGH); // Turns relay ON
    }

    delay(1000);
    unsigned long startMillis = millis();
    while ((millis() - startMillis) < (durationSeconds * 1000)) {
        // Add If Else Case here depending on Node Type
        if(NodeType == "Central Hub"){
            Serial.println(F("Central Hub Programme Running"));
            Serial.println(F("Core 1: Collecting GPS data..."));
            while (GPS_Serial.available() > 0) {
                if (gps.encode(GPS_Serial.read())) {
                    if(debugMode == "On"){
                        Serial.println(F("CENTRAL HUB DEBUG MODE ON"));
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
                      Serial.println(F("NODE CENTRE DEBUG MODE ON"));
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
                      Serial.println(F("SENSOR NODE DEBUG MODE ON"));
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
                      Serial.println(F("SCATTER NODE EXTERNAL DEBUG MODE ON"));
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
                      Serial.println(F("SCATTER NODE INTERNAL DEBUG MODE ON"));
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
                      Serial.println(F("REPEATER NODE DEBUG MODE ON"));
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
                      Serial.println(F("NO NODE TYPE SELECTED DEBUGGING MODE ON"));
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
    goToSleep(300);
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
    
    // Create a task to put Core 1 to sleep
    xTaskCreatePinnedToCore(
        [] (void *pvParameters) {
            delay(1000); // Simulate some time before sleep
            esp_sleep_enable_timer_wakeup(*(int*)pvParameters * 1000000);
            Serial.flush();
            esp_deep_sleep_start(); // Deep sleep Core 1
            vTaskDelete(NULL); // Clean up task
        },
        "SleepTask", // Task name
        10000, // Stack size
        &sleepSeconds, // Parameter to pass to the task (sleep time in seconds)
        1, // Priority
        NULL, // Task handle
        1  // Core 1
    );
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
        Serial.print(F("ERROR: GPS DATE INVALID "));
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
        Serial.print(F("ERROR: GPS TIME INVALID "));
    } 
    if (gps.location.isValid()) {
        Serial.print(gps.location.lat(), 6);
        Serial.print(F(" | "));
        Serial.print(gps.location.lng(), 6);
        Serial.print(F(" | "));
    } else {
        Serial.print(F("ERROR: GPS LOCATION DATA INVALID "));
    }
    if (gps.satellites.isValid()) {
        Serial.print(gps.satellites.value());
    } else {
        Serial.print(F("ERROR: GPS SATELLITES COUNT INVALI "));
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
        Serial.println(F("ERROR: GPS DATE INVALID "));
    }
    if (gps.time.isValid()) {
        gps.time.hour();
        gps.time.minute();
        gps.time.second();
        gps.time.centisecond();
    } else {
        Serial.println(F("ERROR: GPS TIME INVALID "));
    }
    if (gps.location.isValid()) {
        gps.location.lat();
        gps.location.lng();
    } else {
        Serial.println(F("ERROR: GPS LOCATION DATA INVALID "));
    }
}

void startGetGPSTime(){
  if (gps.date.isValid()) {
        Serial.print(gps.date.month());
        Serial.print(F("/"));
        Serial.print(gps.date.day());
        Serial.print(F("/"));
        Serial.print(gps.date.year());
        Serial.print(F(" | "));
    } else {
        Serial.print(F("ERROR: GPS DATE INVALID "));
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
        Serial.println(F(" | "));
    } else {
        Serial.println(F("ERROR: GPS TIME INVALID "));
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


#include "core1_tasks.h"
#include "esp_sleep.h"
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include "nodeSettings.h"
#include "systemSettings.h"
#include "pinSetUp.h"  // Pin Setup and Pin Defnines

// Define the GPS serial port
HardwareSerial mySerial(1);  // Use UART1 (GPIO 33, GPIO 32 for RX/TX)

// Initialize the TinyGPS++ object
TinyGPSPlus gps;

///MARK.....DO NOT FUCKING TOUCH THIS SECTION BELOW - LEAVE IT ALONE, IT WORKS.
void core1Task(void *parameter) {
    int gpsMinute = 0; // Simulated GPS minute input
    unsigned long lastTime = 0;  // Keep track of the last GPS update

    // Start GPS communication
    mySerial.begin(9600, SERIAL_8N1, 33, 32); // Initialize with the correct RX, TX pins

    while (true) {
        while (mySerial.available() > 0) {
            gps.encode(mySerial.read());  // Feed GPS data to the TinyGPS object
        }

        // Check if a new GPS fix is available
        if (gps.location.isUpdated()) {
            // Get the current GPS time (assuming UTC)
            int day = gps.date.day();
            int month = gps.date.month();
            int year = gps.date.year();
            int hour = gps.time.hour();
            int minute = gps.time.minute();
            int second = gps.time.second();
            int centisecond = gps.time.centisecond();
            
            // Print the time from GPS
            Serial.print("GPS Time & Date: ");
            if (gps.date.day() < 10) Serial.print(F("0"));
            Serial.print(day);
            Serial.print(F("/"));
            if (gps.date.month() < 10) Serial.print(F("0"));
            Serial.print(month);
            Serial.print(F("/"));
            Serial.print(year);
            Serial.print(F(" | "));
            if (gps.time.hour() < 10) Serial.print(F("0"));
            Serial.print(hour);
            Serial.print(":");
            if (gps.time.minute() < 10) Serial.print(F("0"));
            Serial.print(minute);
            Serial.print(":");
            if (gps.time.second() < 10) Serial.print(F("0"));
            Serial.print(second);
            Serial.print(":");
            if (gps.time.centisecond() < 10) Serial.print(F("0"));
            Serial.println(centisecond);

            // Set the gpsMinute based on GPS data (just for simulation)
            gpsMinute = minute;  // You can use more precise logic for gpsMinute if needed
        }

        if (shouldStayAwake(gpsMinute, awakeMode)) {
            Serial.println(F("Staying awake..."));
            executeTask(awakeMode);  // Execute tasks while awake
        } else {
            int sleepTime = timeToNextWake(gpsMinute, awakeMode);
            goToSleep(sleepTime);  // Go to sleep after tasks are done
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay between checks
    }
}

bool shouldStayAwake(int minute, String mode) {
    if (mode == "5") {
        return ((minute >= 0 && minute <= 4) || (minute >= 10 && minute <= 14) ||
                (minute >= 20 && minute <= 24) || (minute >= 30 && minute <= 34) ||
                (minute >= 40 && minute <= 44) || (minute >= 50 && minute <= 54));
    } else if (mode == "15") {
        return ((minute >= 0 && minute <= 4) || (minute >= 15 && minute <= 19) ||
                (minute >= 30 && minute <= 34) || (minute >= 45 && minute <= 49));
    } else if (mode == "30") {
        return ((minute >= 0 && minute <= 4) || (minute >= 30 && minute <= 34));
    }
    return false;
}

int timeToNextWake(int minute, String mode) {
    int wakeTimes5[] = {0, 10, 20, 30, 40, 50};
    int wakeTimes15[] = {0, 15, 30, 45};
    int wakeTimes30[] = {0, 30};

    int* wakeTimes;
    int size;

    if (mode == "5") {
        wakeTimes = wakeTimes5;
        size = 6;
    } else if (mode == "15") {
        wakeTimes = wakeTimes15;
        size = 4;
    } else if (mode == "30") {
        wakeTimes = wakeTimes30;
        size = 2;
    } else {
        return 0;
    }

    for (int i = 0; i < size; i++) {
        if (minute < wakeTimes[i]) {
            return (wakeTimes[i] - minute) * 60; // Convert to seconds
        }
    }
    return (60 - minute + wakeTimes[0]) * 60; // Wrap to next hour
}

void executeTask(String mode) {//Execute these tasks when the system is awake.
    if (awakeMode == "5") {
        //Serial.println("Executing short interval tasks...");
        mainProgramme();
        delay(1000);  // Simulating task delay
    } else if (awakeMode == "15") {
        //Serial.println("Executing medium interval tasks...");
        mainProgramme();
        delay(1000);  // Simulating task delay
    } else if (awakeMode == "30") {
        //Serial.println("Executing long interval tasks...");
        mainProgramme();
        delay(1000);  // Simulating task delay
    }
    else{
      Serial.println(F("No Awake Mode Chosen in systemSettings.h"));
    }
}//End of executeTask function.

void goToSleep(int sleepTime) {
    if (debugMode == "On"){
        Serial.println(F("Shutting Node Down...Preparing to go To Sleep "));
        delay(100);
        Serial.println(F("3V Relay Module Switching DEBUG MODE ON "));
        delay(100);
        Serial.println(F("3V Relay Off "));
        digitalWrite(Relay3v, LOW); // Turns relay OFF
        delay(100);
    }else{
      digitalWrite(Relay3v, LOW); // Turns relay OFF
    }
    Serial.println(F("Going to sleep for "));
    delay(100);
    Serial.print(sleepTime);
    Serial.println(F(" seconds..."));
    delay(100);
    esp_sleep_enable_timer_wakeup(sleepTime * 1000000ULL);  // Convert to microseconds
    esp_deep_sleep_start();  // Put the ESP32 into deep sleep
}//End of goToSleep Function
///MARK.....DO NOT FUCKING TOUCH THIS SECTION ABOVE - LEAVE IT ALONE.....

void displayGPSInfoSerial() {
    Serial.println(F("Location Data from GT-U7 GPS Module."));
    if (gps.date.isValid()) {
      if (gps.date.day() < 10) Serial.print(F("0"));
        Serial.print(gps.date.day());
        Serial.print(F("/"));
      if (gps.date.month() < 10) Serial.print(F("0"));
        Serial.print(gps.date.month());
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
}//End of DisplayGPS Info - Serial.print Statements

void mainProgramme(){//This is the Main Programme that will run depending on Node Type.
  if(NodeType == "Central Hub"){
    if (debugMode == "On"){
        Serial.println(F("Running Central Hub Main Programme."));
        Serial.println(F("3V Relay Module Switching DEBUG MODE ON "));
        Serial.println(F("3V Relay On "));
        digitalWrite(Relay3v, HIGH); // Turns relay ON
        delay(50);
    }else{
      digitalWrite(Relay3v, HIGH); // Turns relay ON
    }
    //End of Central Hub mainProgramme
  }
  else if(NodeType == "Node Centre"){
    if (debugMode == "On"){
        Serial.println(F("3V Relay Module Switching DEBUG MODE ON "));
        Serial.println(F("3V Relay On "));
        digitalWrite(Relay3v, HIGH); // Turns relay ON
        delay(50);
    }else{
      digitalWrite(Relay3v, HIGH); // Turns relay ON
    }
    
  }
  else if(NodeType == "Sensor Node"){
    if (debugMode == "On"){
        Serial.println(F("3V Relay Module Switching DEBUG MODE ON "));
        Serial.println(F("3V Relay On "));
        digitalWrite(Relay3v, HIGH); // Turns relay ON
        delay(50);
    }else{
      digitalWrite(Relay3v, HIGH); // Turns relay ON
    }
  }
  else if(NodeType == "Scatter Node Internal"){
    if (debugMode == "On"){
        Serial.println(F("3V Relay Module Switching DEBUG MODE ON "));
        Serial.println(F("3V Relay On "));
        digitalWrite(Relay3v, HIGH); // Turns relay ON
        delay(50);
    }else{
      digitalWrite(Relay3v, HIGH); // Turns relay ON
    }
  }
  else if(NodeType == "Scatter Node External"){
    if (debugMode == "On"){
        Serial.println(F("3V Relay Module Switching DEBUG MODE ON "));
        Serial.println(F("3V Relay On "));
        digitalWrite(Relay3v, HIGH); // Turns relay ON
        delay(50);
    }else{
      digitalWrite(Relay3v, HIGH); // Turns relay ON
    }
  }
  else if(NodeType == "Repeater Node"){
    if (debugMode == "On"){
        Serial.println(F("3V Relay Module Switching DEBUG MODE ON "));
        Serial.println(F("3V Relay On "));
        digitalWrite(Relay3v, HIGH); // Turns relay ON
        delay(50);
    }else{
      digitalWrite(Relay3v, HIGH); // Turns relay ON
    }
  }
  else{
    Serial.print(F("No Node Type Defined ... Check nodeSettings.h header File."));
    delay(1000);
  }

}

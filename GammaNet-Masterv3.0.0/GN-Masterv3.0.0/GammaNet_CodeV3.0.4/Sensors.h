#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

// Define classes for each sensor
class TempSensor {
public:
    TempSensor(int pin);
    void begin();
    float readTemperature();
private:
    int _pin;
};

class HumiditySensor {
public:
    HumiditySensor(int pin);
    void begin();
    float readHumidity();
private:
    int _pin;
};

// GPS Sensor class
class GPSSensor {
public:
    GPSSensor(HardwareSerial &serialPort); // Accepts a HardwareSerial port (e.g., Serial1)
    void begin();
    void update();
    double getLatitude();
    double getLongitude();
    float getAltitude();
    float getSpeed();
private:
    HardwareSerial &_serial;
    TinyGPSPlus gps;
};

#endif

#include "Sensors.h"

// GPS Sensor Implementation
GPSSensor::GPSSensor(HardwareSerial &serialPort) : _serial(serialPort) {}

void GPSSensor::begin() {
    _serial.begin(115200);  // GPS typically runs at 9600 baud, adjust as necessary
}

void GPSSensor::update() {
    while (_serial.available() > 0) {
        gps.encode(_serial.read());
    }
}

double GPSSensor::getLatitude() {
    if (gps.location.isUpdated()) {
        return gps.location.lat();
    }
    return 0.0;
}

double GPSSensor::getLongitude() {
    if (gps.location.isUpdated()) {
        return gps.location.lng();
    }
    return 0.0;
}

float GPSSensor::getAltitude() {
    if (gps.altitude.isUpdated()) {
        return gps.altitude.meters();
    }
    return 0.0;
}

float GPSSensor::getSpeed() {
    if (gps.speed.isUpdated()) {
        return gps.speed.kmph();
    }
    return 0.0;
}

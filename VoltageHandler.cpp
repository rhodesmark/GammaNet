#include "VoltageHandler.h"

void initVoltageSensors() {
  analogReadResolution(12);   // ESP32 default is 12 bits
  pinMode(SOLAR_PIN, INPUT);
  pinMode(BATTA_PIN, INPUT);
  pinMode(BATTB_PIN, INPUT);
}

float readVoltage(int pin) {
  int raw = analogRead(pin);
  float voltage = (raw / ADC_RESOLUTION) * ADC_REF_VOLTAGE * VOLTAGE_DIVIDER_RATIO;
  return voltage;
}

float readSolarVoltage() {
  return readVoltage(SOLAR_PIN);
}

float readBatteryAVoltage() {
  return readVoltage(BATTA_PIN);
}

float readBatteryBVoltage() {
  return readVoltage(BATTB_PIN);
}

String getVoltageCSV() {
  float solarV = readSolarVoltage();
  float battA  = readBatteryAVoltage();
  float battB  = readBatteryBVoltage();

  // CSV fragment (will be appended to GPS CSV)
  String data = String(solarV, 2) + "," + String(battA, 2) + "," + String(battB, 2);
  return data;
}

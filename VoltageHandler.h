#ifndef VOLTAGE_HANDLER_H
#define VOLTAGE_HANDLER_H

#include <Arduino.h>

// Define ADC pins for voltage measurement
#define SOLAR_PIN   34
#define BATTA_PIN   35
#define BATTB_PIN   36

// Voltage Divider Ratios (adjust to your resistors)
#define VOLTAGE_DIVIDER_RATIO 2.0f   // e.g. if R1=10k, R2=10k → ratio = 2

// Reference voltage for ESP32 ADC (approx. 3.3V, calibrate as needed)
#define ADC_REF_VOLTAGE 3.3f
#define ADC_RESOLUTION  4095.0f

void initVoltageSensors();
float readSolarVoltage();
float readBatteryAVoltage();
float readBatteryBVoltage();
String getVoltageCSV();

#endif

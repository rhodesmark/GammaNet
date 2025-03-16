#include "Arduino.h"

//HC-12 Communication Module Define Pins
#define hc12_TX 26// ESP32 TX (connect to HC-12 RX)
#define hc12_RX 27  // ESP32 RX (connect to HC-12 TX)
#define hc12_SET 25 // ESP32 GPIO to control SET mode
//1 Channel Relay Module
#define RELAY1_1 15
//4 Channel Relay Module
#define RELAY4_1 4
#define RELAY4_2 16
#define RELAY4_3 17
#define RELAY4_4 5
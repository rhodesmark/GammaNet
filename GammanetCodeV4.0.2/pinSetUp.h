#include "Arduino.h"

#define MODEM_BAUDRATE                      (115200)
#define MODEM_DTR_PIN                       (25)
#define MODEM_TX_PIN                        (26)
#define MODEM_RX_PIN                        (27)
// The modem boot pin needs to follow the startup sequence.
#define BOARD_PWRKEY_PIN                    (4)
#define BOARD_BAT_ADC_PIN                   (35)
// The modem power switch must be set to HIGH for the modem to supply power.
#define BOARD_POWERON_PIN                   (12)        //T-A7608-V2 is onboard led
#define MODEM_RING_PIN                      (33)
#define MODEM_RESET_PIN                     (5)         //T-A7608-V2 no connection

#define BOARD_MISO_PIN                      (2)
#define BOARD_MOSI_PIN                      (15)
#define BOARD_SCK_PIN                       (14)
#define BOARD_SD_CS_PIN                     (13)

#define MODEM_RESET_LEVEL                   HIGH
#define SerialAT                            Serial1


#ifndef TINY_GSM_MODEM_A7608
#define TINY_GSM_MODEM_A7608
#endif

// only version v1.1 or V2  has solar adc pin
#define BOARD_SOLAR_ADC_PIN                 (34)

// 127 is defined in GSM as the AUXVDD index
#define MODEM_GPS_ENABLE_GPIO               (127)
#define MODEM_GPS_ENABLE_LEVEL              (1)

//SD Card Pin Definitions:
// Define SPI pins for MicroSD For ESP32-Wroom-32 Boards Only.
////#define SD_CS 13  // Chip Select (CS)
//#define SD_SCK 2  // Clock (SCK)
//#define SD_MISO 15  // MISO
//#define SD_MOSI 13 // MOSI

//3v Relay Power Pin Definitions
#define Relay3v 22  // Relay 3v Switching
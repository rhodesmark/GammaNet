#include "GPSHandler.h"

TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

void initGPS() {
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
}

void readGPS() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }
}

String getGPSData() {
  if (gps.location.isValid() && gps.date.isValid() && gps.time.isValid()) {
    String dateStr = String(gps.date.day()) + "/" +
                     String(gps.date.month()) + "/" +
                     String(gps.date.year());

    char timeStr[12];
    sprintf(timeStr, "%02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second());

    String latStr  = String(gps.location.lat(), 6);
    String lngStr  = String(gps.location.lng(), 6);
    String satsStr = String(gps.satellites.value());

    // CSV fragment: Date,Time,Lat,Lng,Sats
    String data = dateStr + "," + timeStr + "," + latStr + "," + lngStr + "," + satsStr;
    return data;
  } else {
    return "NoFix,NoFix,0.000000,0.000000,0";
  }
}

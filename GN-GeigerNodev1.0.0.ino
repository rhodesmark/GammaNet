#include <painlessMesh.h>
#include "GPSHandler.h"
#include "VoltageHandler.h"

// ----- Mesh Config -----
#define MESH_PREFIX     "GammaNet-Network"
#define MESH_PASSWORD   "977afc379e96b294876160bea8eb79cf3396f554"
#define MESH_PORT       5555

const String NODE_ID = "GN103";  // Unique Node ID for this sensor
const String NODE_TYPE = "GN";   // Node type for root identification

painlessMesh mesh;

unsigned long lastSend = 0;
const unsigned long SEND_INTERVAL = 5000; // send data every 5 seconds

uint32_t rootId = 0;
bool rootFound = false;

// ----- Callbacks -----
void receivedCallback(const uint32_t &from, const String &msg) {
  if (msg.startsWith("ROOT_HEARTBEAT")) {
    if (!rootFound) {
      rootId = from;
      rootFound = true;
      Serial.printf("Root found with mesh ID: %u\n", rootId);

      String initMsg = "TYPE:" + NODE_TYPE + " NodeID:" + NODE_ID + " Data:INIT";
      mesh.sendSingle(rootId, initMsg);
      Serial.printf("Sent initial type message to root: %s\n", initMsg.c_str());
    }
  } else {
    Serial.printf("Received from %u: %s\n", from, msg.c_str());
  }
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("New node connected: %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.println("Mesh connections changed");
}

// ----- Setup -----
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting Geiger Node with GPS + Voltage Sensors...");

  initGPS();
  initVoltageSensors();

  mesh.init(MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
}

// ----- Loop -----
void loop() {
  mesh.update();
  readGPS();

  if (rootFound && millis() - lastSend > SEND_INTERVAL) {
    lastSend = millis();

    // Build CSV output
    String gpsData = getGPSData();       // Date,Time,Lat,Lng,Sats
    String voltData = getVoltageCSV();   // SolarV,BattA,BattB
    String csvLine = gpsData + "," + voltData;

    String msg = "NodeID:" + NODE_ID + " Data:" + csvLine;
    mesh.sendSingle(rootId, msg);

    Serial.printf("Sent to root: %s\n", msg.c_str());
  }
}

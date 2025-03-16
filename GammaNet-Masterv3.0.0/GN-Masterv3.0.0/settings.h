#include "Arduino.h"

const String SystemName = "GammaNet";
const String CodeVersion = "2.0.0";
const String CodeDeveloper = "Mark Rhodes";
//Node Information
const String DeviceType = "ESP32-32";
const String NodeType = "Central Hub"; //Central Hub, Node Center, Sensor Node, Repeater Node, Scatter Node External
const String NodeID = "CH001";
const String NodeStyle = "External"; //Internal, External
//Internet Connection Details
const String WebURL = "https://dataIn.gammanet.co.uk";
const String phpFile1 = "dataSubmit.php";
const String fullURL1 = "https://dataIn.gammanet.co.uk/dataSubmit.php";
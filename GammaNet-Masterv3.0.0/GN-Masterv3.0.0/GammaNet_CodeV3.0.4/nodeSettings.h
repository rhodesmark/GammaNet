#include "Arduino.h"

const String SystemName = "GammaNet";
const String CodeVersion = "3.0.4";
const String CodeDeveloper = "Mark Rhodes";
//Node Information
const String NodeType = "Central Hub"; //Central Hub, Node Centre, Sensor Node, Repeater Node, Scatter Node.
const String NodeID = "CH001";
const String NodeStyle = "External"; //Internal, External
//Internet Connection Details
const String WebURL = "https://dataIn.gammanet.co.uk";
const String phpFile1 = "dataSubmit.php";
const String fullURL1 = "https://dataIn.gammanet.co.uk/dataSubmit.php";
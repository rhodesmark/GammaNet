#include "Arduino.h"

const String SystemName = "GammaNet";
const String CodeVersion = "4.0.2 Beta";
const String CodeDeveloper = "Mark Rhodes";
//Node Information
const String NodeType = "Central Hub"; //Central Hub, Node Centre, Sensor Node, Repeater Node, Scatter Node.
const String NodeID = "CH001";
const String NodeStyle = "External"; //Internal, External

//Internet Connection Details for the Central hub
const String WebURL = "https://dataIn.gammanet.co.uk";
const String phpFile1 = "dataSubmit.php";
const String fullURL1 = "https://dataIn.gammanet.co.uk/dataSubmit.php";
const String phpFile2 = "conectTest.php";
const String fullURL2 = "https://dataIn.gammanet.co.uk/connectTest.php";
#include <WiFi.h>
#include <EEPROM.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <esp_timer.h>
#include <esp_camera.h>
#include "img_converters.h"
#include "Arduino.h"

//Settings
#define Off 0
byte On = !Off;
String serverName1 = "black0box.000webhostapp.com";   // REPLACE WITH YOUR Domain Name
#define FlashON 0 //Flash On/off Switch while Streaming
const char* defssid = "Setup_Mode"; //Set UP SSID
const char* defpass = "123456789";  //Set UP Password
byte lps = 5;  //Tick Rate
byte lpc = 15;  //Interval Ticks Between Requests 
#define Threshold 8000 //Hold Down Period For Reseting


//Pins
#define resBut 12
#define lock 14
#define lockfeedback 2
#define PIR 16
#define Gled 13
#define Rled 15
#define Lights 4

unsigned long startTime;

int i = 0;
int statusCode;
String content;
String esid,epass;
WebServer server(80);

byte val[] = {0, 0, 0, 0, 0, 0}; //DataBase Values
int counter; //Tick Counter



String ckURL[] = {"getMotionSensorEnabled.php", "getIsStreaming.php", "getLockstatus.php"};
String stURL[] = {"setMotionSensor.php", "setFrame.php", "setLockstatus.php", "setdoorstatus.php"};
byte bstate = 1;
unsigned long bst = 0;

WiFiClient client;

byte streaming = 0;
String serverName = "http://" + serverName1 + "/";
String serverPath = "/upload.php";

IPAddress Ip(192, 168, 4, 1);
IPAddress NMask(255, 255, 255, 0);

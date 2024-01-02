#include "AS726X.h"                   // AS7262 Library
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Wire.h>
#include <Arduino_JSON.h>

const char* ssid = "SGU-LAB";
const char* password = "SGUnumber1";

AsyncWebServer server(80);
AsyncEventSource events("/events");

AS726X sensor;    // Initialize Lirbary
JSONVar readings;
const int LDRPin = 32;
float val, lit;

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

void setup_wifi() 
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

String getSensorReadings()
{ 
  sensor.takeMeasurements();

  readings["violet"] = String(sensor.getCalibratedViolet(), 2);
  readings["blue"] = String(sensor.getCalibratedBlue(), 2);
  readings["green"] = String(sensor.getCalibratedGreen(), 2);
  readings["yellow"] = String(sensor.getCalibratedYellow(), 2);
  readings["orange"] = String(sensor.getCalibratedOrange(), 2);
  readings["red"] = String(sensor.getCalibratedRed(), 2);

  String jsonString = JSON.stringify(readings);
  return jsonString;
}



void setup() 
{
  Wire.begin();
  Serial.begin(115200);
  sensor.begin();

  bool status; 
  if(!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  setup_wifi();             // Connect to Wifi

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");
  

  // Request for the latest sensor readings
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    lastTime = lastTime + timerDelay;
    request->send(200, "text/html", "OK!");
  });

  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  // Start server
  server.begin();
}


void loop() 
{
    if ((millis() - lastTime) > timerDelay) 
    {
    // Send Events to the client with the Sensor Readings Every 10 seconds
    events.send("ping",NULL,millis());
    events.send(getSensorReadings().c_str(),"new_readings" ,millis());
    lastTime = millis();
    }
}

#include <ESP32Servo.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Stepper.h>
#include "AS726X.h"                   // AS7262 Library
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Arduino_JSON.h>

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
const char* ssid = "SGU-LAB";
const char* password = "SGUnumber1";

AsyncWebServer server(80);
AsyncEventSource events("/events");

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

AS726X sensor;    // Initialize Lirbary
JSONVar readings;

#define CLK 13
#define DT 14

// ULN2003 Motor Driver Pins
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33
#define BUTTON_PIN 35

int servoPin = 32;
int pos = 0;
int counter = 0;
int currentStateCLK;
int lastStateCLK;
int relay = 4;
int button;        
int previous = LOW;
unsigned long debounce = 200UL;
unsigned long tim = 0;

int lastButtonState;    // the previous state of button
int currentButtonState; // the current state of button

float GreenM = 53798.32;
float OrangeM = 47655.52;
float YellowM = 46668.28;
float RedM = 2653.8;
float VioletM = 42821.49;
float BlueM = 1734.25; 
 
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);
Servo servo;
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);


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
  float violet, blue, green, yellow, orange, red;

  violet = sensor.getCalibratedViolet();
  blue = sensor.getCalibratedBlue();
  green = sensor.getCalibratedGreen();
  yellow = sensor.getCalibratedYellow();
  orange = sensor.getCalibratedOrange();
  red = sensor.getCalibratedRed();
  Serial.println(violet);

  readings["violet"] = String(VioletM - violet, 2);
  readings["blue"] = String(BlueM - blue, 2);
  readings["green"] = String(GreenM - green, 2);
  readings["yellow"] = String(YellowM - yellow, 2);
  readings["orange"] = String(OrangeM - orange, 2);
  readings["red"] = String(RedM - red, 2);

  String jsonString = JSON.stringify(readings);
  return jsonString;
}

void setup() 
{
  Serial.begin(115200); 
  Wire.begin();
  sensor.begin();
  pinMode(BUTTON_PIN, INPUT);         // Button Pin
  
  pinMode(CLK,INPUT);                 // Encoder CLK
  pinMode(DT,INPUT);                  // Encoder DT
  
  servo.attach(servoPin);             // ServoPin
  servo.write(counter);
  lastStateCLK = digitalRead(CLK);
  
  pinMode(relay, OUTPUT);             // Relay Pin

  myStepper.setSpeed(10);             // Stepper Speed (in rpm)
  
  lcd.begin(16, 2);                   // LCD Setup
  lcd.setCursor(0, 0);  
  lcd.print("Spectro Project");                    // LCD 
  lcd.setCursor(0, 1);                // LCD Cursor
  lcd.print("BE 2021");  

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

  events.onConnect([](AsyncEventSourceClient *client)
  {
    if(client->lastId())
    {
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
  });
  
  server.addHandler(&events);

  // Start server
  server.begin();
}


void loop() 
{ 
  button = digitalRead(BUTTON_PIN);
  Serial.println("Button State: ");
  Serial.println(button);
  
  while (button == LOW) 
  {
    ServoEncoder();
  }
  
    Serial.println("Light On");
    digitalWrite(relay, LOW);

    for (int i = 0; i < 6; i++)
    {
      Serial.println("Taking Data"); 
     
      for(int steps = 0; steps < 344; steps++)   // 344 disini dari 2046/360 x 60, for 60 degree rotation
      { 
        myStepper.step(1);                 // do one step
      }

      events.send("ping", NULL, false);
      events.send(getSensorReadings().c_str(),"new_readings" , false);
      
      delay(2000);
    }
    
    Serial.println("Data Taken");
    digitalWrite(relay, HIGH);
}


void ServoEncoder()
{
  currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1)
  {
    if (digitalRead(DT) != currentStateCLK) 
    {
      counter --;
      if (counter < 0)
        counter = 0;
    } 
    else 
    {
      counter ++;
      if (counter > 179)
        counter = 179;
    }
    servo.write(counter);
    lcd.setCursor(0, 1);
    lcd.print("Position: ");
    lcd.print(counter);
  } 
  
  lastStateCLK = currentStateCLK;

  button = digitalRead(BUTTON_PIN);
  Serial.println("Button State: ");
  Serial.println(button);
}

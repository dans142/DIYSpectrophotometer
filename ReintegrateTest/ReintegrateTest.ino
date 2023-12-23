
#include <ESP32Servo.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Stepper.h>

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution

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
unsigned long debounce = 200UL;
unsigned long tim = 0;
int lastButtonState;    // the previous state of button
int currentButtonState; // the current state of button
 
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);
Servo servo;
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);


void setup() 
{
  Serial.begin(115200); 

  pinMode(BUTTON_PIN, INPUT);         // Button Pin
  currentButtonState = digitalRead(BUTTON_PIN);
  
  pinMode(CLK,INPUT);                         // Encoder CLK
  pinMode(DT,INPUT);                          // Encoder DT
  
  servo.attach(servoPin);                     // ServoPin
  servo.write(counter);
  lastStateCLK = digitalRead(CLK);
  
  pinMode(relay, OUTPUT);                     // Relay Pin

  myStepper.setSpeed(10);                     // Stepper Speed (in rpm)
  
  lcd.begin(16, 2);                           // LCD Setup
  lcd.print("Spectro Project");               // LCD 
  lcd.setCursor(0, 1);                        // LCD Cursor
  lcd.print("BE 2021");  
}


void loop() 
{
  lastButtonState    = currentButtonState;
  currentButtonState = digitalRead(BUTTON_PIN);
  Serial.println("Button State: ");
  Serial.print(currentButtonState);

if(lastButtonState == LOW && currentButtonState == HIGH) 
{
    Serial.println("The button is pressed");
    Serial.println("Light On");
    digitalWrite(relay, LOW); 

    for (int i = 0; i < 6; i++)
    {
      Serial.println("Taking Data"); 
      for(int steps = 0; steps < 341; steps++)   // 341 disini dari 2048/360 x 60, for 60 degree rotation
        { // do one rotation
          myStepper.step(1);                     // do one step
        }
    }
}
    else
      ServoEncoder();
}


void ServoEncoder()
{
  currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
    if (digitalRead(DT) != currentStateCLK) {
      counter --;
      if (counter < 0)
        counter=0;
    } else {
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
}

// Libraries Used
#include <ESP32Servo.h>   
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Stepper.h>

const int stepsPerRevolution = 2048;  // Total Steps for 1 revolution

// ULN2003 Motor Driver Pins
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33
// Encoder Pins
#define CLK 13
#define DT 14

int servoPin = 32;    // Servo Pin     
int counter = 0;      // Counter as per Encoder Usage
int currentStateCLK;  // Comparator
int lastStateCLK;     // Comparator
 
// initialize libraries
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);
Servo servo;
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

void setup() 
{
  Serial.begin(115200); 
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  servo.attach(32);
  servo.write(counter);
  lastStateCLK = digitalRead(CLK);
  lcd.begin(16, 2);
  lcd.print("Test");   

  myStepper.setSpeed(10);
}
void loop() 
{ 
  ServoEncoder();     // ServoEncoder() selfmade function

void ServoEncoder()
{
  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK);
  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentStateCLK) {
      counter --;
      if (counter < 0)
        counter=0;
    } else {
      // Encoder is rotating CW so increment
      counter ++;
      if (counter > 180)
        counter = 180;
    }
    // Move the servo
    servo.write(counter);
    lcd.setCursor(0,1);
    lcd.print("Position: ");
    lcd.print(counter);
  }
  // Remember last CLK state
  lastStateCLK = currentStateCLK;
}

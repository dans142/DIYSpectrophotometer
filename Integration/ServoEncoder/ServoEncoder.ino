#include <ESP32Servo.h>     // Servo Library

// Rotary Encoder Inputs
#define CLK 13
#define DT 14

Servo servo;    // Initialize Servo Library

int counter = 0;      // Variable for counter, angle of servo
int currentStateCLK;  // Comparator for Encoder
int lastStateCLK;     // Comparator for Encoder

void setup() 
{
  // Set encoder pins as inputs
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  Serial.begin(9600);         // Setup Serial Monitor
  servo.attach(32);           // Attach servo on pin 32 to the servo object
  servo.write(counter);  
  lastStateCLK = digitalRead(CLK);    // Read the initial state of CLK pin
}

void loop() 
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
      if (counter > 179)
        counter = 179;
    }
    // Move the servo
    servo.write(counter);
    Serial.print("Position: ");
    Serial.println(counter);
  }
  // Remember last CLK state
  lastStateCLK = currentStateCLK;
}

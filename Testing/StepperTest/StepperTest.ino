#include <Stepper.h>    // Stepper Library 

const int stepsPerRevolution = 2048;    // Total Steps for 1 revolution

// Stepper Motor Pins
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33

// Initialize Library
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

void setup() 
{
  myStepper.setSpeed(10);     // Stepper Motor RPM
  Serial.begin(115200);       // Initiate Serial Monitor 
}

void loop() 
{
  turns();    // Turns() Function to rotate Stepper Motor
}

void turns()    // Definition of Turns() Function
{
     for(int steps = 0; steps < 100; steps++)   // Loop for 100 steps each cycle
     { 
         myStepper.step(1);                   // do one step
     }
  delay(1000);
}

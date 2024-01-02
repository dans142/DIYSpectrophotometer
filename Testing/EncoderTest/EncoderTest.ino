#include <ESP32Servo.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Stepper.h>
#include <ESP32Encoder.h>

ESP32Encoder encoder;

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution

// ULN2003 Motor Driver Pins
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33

int servoPin = 32;
int pos = 0;
int counter = 0;
int currentStateCLK;
int lastStateCLK;
 
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);
Servo servo;
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

#define CLK 13
#define DT 14

void setup() 
{
  Serial.begin(115200); 
  encoder.attachHalfQuad ( DT, CLK );
  encoder.setCount ( 0 );
  servo.attach(32);
  servo.write(counter);
  lastStateCLK = digitalRead(CLK);
  lcd.begin(16, 2);
  lcd.print("Test");   

  myStepper.setSpeed(10);
}
void loop() 
{ 
  ServoEncoder();
}

void ServoEncoder()
{
  long newPosition = encoder.getCount() / 2;
    if (newPosition < 0) {
      newPosition = 0;
    } else if (newPosition > 180)
    {
      newPosition = 180;
    }

    Serial.println(newPosition);
    servo.write(newPosition);
    lcd.setCursor(0,1);
    lcd.print("Position: ");
    lcd.print(newPosition);
}

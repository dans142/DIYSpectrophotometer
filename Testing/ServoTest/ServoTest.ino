/* Servo Test
Green => GND
Blue => Source
Orange => 32
 */

#include <ESP32Servo.h>   // Servo Library
Servo servo1;   // Initializing Servo Library

int servoPin = 32;    // Servo Pin

void setup() 
{
  // Servo Pin Assignment via Library function
  servo1.attach(servoPin);
}
void loop() 
{ 
  /* The servo is order to do a 360 degree loop to 
     test its limitations and speed */
  servo1.write(0);
  delay(1000);
  servo1.write(60);
  delay(1000);
  servo1.write(120);
  delay(1000);
  servo1.write(180);
  delay(1000);
  servo1.write(240);
  delay(1000);
  servo1.write(300);
  delay(1000);
}

/*  Relay Test 
 *  Relay              
 *  Normal Open ->  Power Source
 *  Common      ->  Light Source -> Power Source
 * 
 *  Input -> Pin 4 ESP32
 *  VCC   -> VCC
 *  GND   -> GND
 */

int relay = 4;      // Relay Pin Assignment

void setup() 
{
  Serial.begin(9600);       // Initialize Serial Monitor
  pinMode(relay, OUTPUT);   // Relay Pin as Output of ESP32
}

void loop() 
{
  // Loops relay on and off every 5 seconds
  digitalWrite(relay, LOW);
  Serial.println("Relay On");
  delay(5000);
  digitalWrite(relay, HIGH);
  Serial.println("Relay Off");
  delay(5000);
}

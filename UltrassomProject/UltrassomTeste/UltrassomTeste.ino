#include <Ultrasonic.h>

/*
 * Pass as a parameter the trigger and echo pin, respectively,
 * or only the signal pin (for sensors 3 pins), like:
 * Ultrasonic ultrasonic(13);
 */
Ultrasonic ultrasonic(12, 13);
int distance;
int led = 8;

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Pass INC as a parameter to get the distance in inches
  distance = ultrasonic.read();
  Serial.println(distance, DEC);

  if (Serial.available()>0){
      char command = Serial.read();
      if(command == 'a'){
          digitalWrite(led, HIGH);
      }
  }
  else{
     digitalWrite(led, LOW);
  }
  delay(50);
}

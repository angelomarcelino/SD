#include <Ultrasonic.h>

/*
   Pass as a parameter the trigger and echo pin, respectively,
   or only the signal pin (for sensors 3 pins), like:
   Ultrasonic ultrasonic(13);
*/
Ultrasonic ultrasonic(12, 13);
int distance;
int ledPerto = 11;
int ledMedio = 10;
int ledLonge = 9;
int incomingInt = 0;
const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data
boolean newData;

void setup() {
  pinMode(ledPerto, OUTPUT);
  pinMode(ledMedio, OUTPUT);
  pinMode(ledLonge, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // Pass INC as a parameter to get the distance in inches
  distance = ultrasonic.read();
  Serial.println(distance, DEC);
  /*
  while (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'p') {
      recvWithEndMarker();
      incomingInt = getNewNumber();
      analogWrite(ledPerto, incomingInt);
      command = Serial.read();
      if (command == 'm') {
        recvWithEndMarker();
        incomingInt = getNewNumber();
        analogWrite(ledMedio, incomingInt);
        command = Serial.read();
        if (command = 'l') {
          recvWithEndMarker();
          incomingInt = getNewNumber();
          analogWrite(ledLonge, incomingInt);
        }
        else return;
      }
      else return;
    }
    else return;
  }
  */
  while(Serial.available() > 0) {
    char command = Serial.read();
    if(command == 'p'){  
      recvWithEndMarker();
      incomingInt = getNewNumber();
      analogWrite(ledPerto, incomingInt);
    }
  }
  
}

void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (Serial.available() > 0) {
    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

int getNewNumber() {
  int dataNumber = 0;
  if (newData == true) {
    dataNumber = atoi(receivedChars);
    newData = false;
  }
  return dataNumber;
}

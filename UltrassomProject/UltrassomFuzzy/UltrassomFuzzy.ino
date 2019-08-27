#include <Ultrasonic.h>
#define BTN_PIN 2
#define POT_PIN 0
#define LED_PERTO 11
#define LED_MEDIO 10
#define LED_LONGE 9
#define MOTOR_PIN1 3
#define MOTOR_PIN2 4
#define HEADLIGHT_LEFT 5
#define HEADLIGHT_RIGHT 6

Ultrasonic ultrasonic(12, 13);

int distance = 0;
int potVal = 0;

String perto, medio, longe, motor;

bool shouldRun = true;

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_PERTO, OUTPUT);
  pinMode(LED_MEDIO, OUTPUT);
  pinMode(LED_LONGE, OUTPUT);

  pinMode(HEADLIGHT_LEFT, OUTPUT);
  pinMode(HEADLIGHT_RIGHT, OUTPUT);

  pinMode(BTN_PIN, INPUT);
  attachInterrupt(
    digitalPinToInterrupt(BTN_PIN), 
    []() { shouldRun = !shouldRun; }, 
    RISING
  );
}

void loop() {
  if(shouldRun) { 
    distance = ultrasonic.read();
    Serial.println(distance, DEC);
  
    while(Serial.available() > 0) {
  
      if(Serial.read() == 'd') {
        perto = Serial.readStringUntil(';');
        medio = Serial.readStringUntil(';');
        longe = Serial.readStringUntil(';');
        motor = Serial.readStringUntil(';'); 
      
        analogWrite(LED_PERTO, perto.toInt());
        analogWrite(LED_MEDIO, medio.toInt());
        analogWrite(LED_LONGE, longe.toInt());

        analogWrite(MOTOR_PIN1, motor.toInt());
        digitalWrite(MOTOR_PIN2, 0);
      }
    }

    potVal = analogRead(POT_PIN);
    analogWrite(HEADLIGHT_LEFT, potVal/4);
    analogWrite(HEADLIGHT_RIGHT, potVal/4);
    
  } else {
    analogWrite(LED_PERTO, 0);
    analogWrite(LED_MEDIO, 0);
    analogWrite(LED_LONGE, 0);
    
    analogWrite(HEADLIGHT_LEFT, 0);
    analogWrite(HEADLIGHT_RIGHT, 0);

    analogWrite(MOTOR_PIN1, 0);
    digitalWrite(MOTOR_PIN2, 0);
  }
}
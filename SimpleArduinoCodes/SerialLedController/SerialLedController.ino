int LED = 11;

void setup() {

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  
  Serial.begin(9600);
  
}


void loop() {

  if (Serial.available() > 0){

    char letter = Serial.read();

    if (letter == 'a'){
      digitalWrite(LED, HIGH);
      Serial.println("ON");
    }
    if (letter == 'd'){
      digitalWrite(LED, LOW);
      Serial.println("OFF");
    }
  }
  
}

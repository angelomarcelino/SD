int i = 10;

void setup() {
  pinMode(i, OUTPUT);
}

void loop() {
  for (int value=0; value < 256; value++){
    analogWrite(i, value);
    delay(1);
  }
    for (int value=255; value > -1; value--){
    analogWrite(i, value);
    delay(1);
  }
  
}

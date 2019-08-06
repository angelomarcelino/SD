
int led = 11;

void setup() {
  
  pinMode(led, OUTPUT);

}

void loop() {

  sos(led, 5);
  
}

void sos(int led, int interval){

  // S(...) tres pontos
  for(int x=0;x<3;x++) // Repete tudo dentro das chaves tres vezes
  {
    digitalWrite(led,HIGH);     // Acende o LED       
    delay(150);                     // Aguarda
    digitalWrite(led,LOW);      // Desliga o LED
    delay(100);                     // Aguarda novamente
  }
   
  delay(200);
       
  // O(---) tres linhas
  for(int x=0;x<3;x++)
  {
    digitalWrite(led,HIGH);            
    delay(450);                           
    digitalWrite(led,LOW);             
    delay(100);                           
  }
 
  delay(200);
       
  //S(...) tres pontos
  for(int x=0;x<3;x++)
  {
    digitalWrite(led,HIGH);           
    delay(150);                           
    digitalWrite(led,LOW);            
    delay(100);                           
  }
 
  delay(interval*1000);
}

#include <LiquidCrystal.h>

// initialize the library
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
            
byte customChar[8] = {
  0b00100,
  0b00010,
  0b00111,
  0b01101,
  0b11111,
  0b10111,
  0b10100,
  0b00011
};

byte customChar1[8] = {
  0b00100,
  0b01000,
  0b11100,
  0b10110,
  0b11111,
  0b11101,
  0b00101,
  0b11000
};

void setup()
{
  Serial.begin(9600);
  // create a new custom character
  lcd.createChar(0, customChar);
  lcd.createChar(1, customChar1);
  // set up number of columns and rows
  lcd.begin(16, 2);

  // print the custom char to the lcd
  // why typecast? see: http://arduino.cc/forum/index.php?topic=74666.0
}

void loop()
{
  lcd.setCursor(5,0);
  lcd.write((uint8_t)0);
  lcd.setCursor(6,0);
  lcd.write((uint8_t)1);
  if(Serial.available() > 0){
    char letter = Serial.read();
   if(letter == 'a'){
    lcd.scrollDisplayLeft();
    delay(300); 
    Serial.println("LEFT");
   }
   if(letter == 'd'){
    lcd.scrollDisplayRight();
    delay(300);
    Serial.println("RIGHT");    
   }
  }
}

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define RISING 3
#define LCD_RS 12
#define LCD_EN 11
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7

char message[16];
volatile byte rx_byte = 0;
volatile int bit_position = 0;
volatile bool update_lcd = true;

ISR(INT0_vect) {
    bool rx_bit = PIND & 0b00001000;

    if (bit_position == 8) {
        rx_byte = 0;
        bit_position = 0;
    }

    if (rx_bit) 
        rx_byte |= (0x80 >> bit_position);
    
    bit_position += 1;

    if (bit_position == 8) 
        strncat(message, (const char *)&rx_byte, 1);

    update_lcd = true;
}

int main(void) {

    LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);

    /*
        RX_CLOCK na porta D2
        RX_DATA na porta D3
    */
    DDRD &= 0b11110011;

    // Limpa a mensagem
    strcpy(message, "");

    // Definindo o interrupt no pino 2 em boarda de subida
    EICRA = (EICRA & ~((1 << ISC00) | (1 << ISC01))) | (RISING << ISC00);
    EIMSK |= (1 << INT0);
    sei(); 

    while(true) {
        // Update LCD
        if (update_lcd) {
            update_lcd = false;
            
            lcd.noCursor();
            lcd.setCursor(0, 0);
            lcd.print(message);

            lcd.setCursor(0, 1);
            for (uint8_t i = 0; i < 8; i++) {
                if (i < bit_position)
                    lcd.print(rx_byte & (0x80 >> i) ? "1" : "0");
                else 
                    lcd.print(" ");
            }

            lcd.setCursor(strlen(message), 0);
            lcd.cursor();
        }
    }

    return 0;
}

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <util/delay.h>
// #include "Display.h"

// Transmit rate in bps
#define TX_RATE 5
#define LCD_RS 6
#define LCD_EN 7
#define LCD_D4 2
#define LCD_D5 3
#define LCD_D6 4
#define LCD_D7 5

const char *message = "Vai dar certo !!";

int main(void) {

    // Incia o LCD
    LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print(message);

    /*
        TX_CLOCK na porta D8
        TX_DATA na porta D9
    */
    DDRB  |= 0b00000011;
    PORTB |= 0b00000000;

    for (uint8_t byte_idx = 0; byte_idx < strlen(message); byte_idx++) {
        char tx_byte = message[byte_idx];

        // Limpa a segunda do display
        lcd.noCursor();
        lcd.setCursor(0, 1);
        lcd.print("        ");
        lcd.setCursor(byte_idx, 0);
        lcd.cursor();

        for (uint8_t bit_idx = 0; bit_idx < 8; bit_idx++) {
            bool tx_bit = tx_byte & (0x80 >> bit_idx);    

            // Envia o bit
            if(tx_bit)
                PORTB |= 0b00000010;
            else
                PORTB &= 0b01111101;

            _delay_ms((1000 / TX_RATE) / 2);

            // Atualiza o LCD
            lcd.noCursor();
            lcd.setCursor(bit_idx, 1);
            lcd.print(tx_bit ? "1" : "0");
            lcd.setCursor(byte_idx, 0);
            lcd.cursor();

            // Pulsa o clock
            PORTB |= 0b00000001;
            _delay_ms((1000 / TX_RATE) / 2);
            PORTB &= 0b01111110;
        }
    }

    // Encerra a transmissão
    PORTB &= 0b01111100; 

    return 0;
}
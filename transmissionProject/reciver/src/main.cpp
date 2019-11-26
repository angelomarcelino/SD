#include <Arduino.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define RISING 3

char message[16];
volatile byte rx_byte = 0;
volatile int bit_position = 0;
// volatile bool update_lcd = true;

ISR(INT0_vect) {
    bool rx_bit = PIND & 0b00000100;

    if (bit_position == 8) {
        rx_byte = 0;
        bit_position = 0;
    }

    if (rx_bit) 
        rx_byte |= (0x80 >> bit_position);
    
    bit_position += 1;

    if (bit_position == 8) 
        strncat(message, (const char *)&rx_byte, 1);

    // update_lcd = true;
}

int main(void) {

    /*
        RX_CLOCK na porta D3
        RX_DATA na porta D2
    */
    DDRD  &= 0b11110011;
    PORTD |= 0b00000000;

    // Limpa a mensagem
    strcpy(message, "");

    // Definindo o interrupt no pino 2 em boarda de subida
    EICRA = (EICRA & ~((1 << ISC00) | (1 << ISC01))) | (RISING << ISC00);
    EIMSK |= (1 << INT0);

    while(true) {
        // Update LCD
    }

}

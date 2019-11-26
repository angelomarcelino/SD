#include <Arduino.h>
#include <util/delay.h>
#include "Display.h"

// Transmit rate in bps
#define TX_RATE 5

const char *message = "Hello, world!";

void myDelay(uint16_t dly) {
    // Define o valor alvo para 62499
    OCR1A = TCNT1 + dly - 1;

    // Define o presclaer do timer1 como 256
    TCCR1B |= (1 << WGM12) | (1 << CS12);

    // Espera pelo evento de overflow do contador 
    while ((TIFR1 & (1 <<  OCF1A)) == 0);

    // Limpa a flag
    TIFR1 |= (1<<OCF1A);  
}

int main(void) {

    /*
        TX_CLOCK na porta D8
        TX_DATA na porta D9
    */
    DDRB  |= 0b00000011;
    PORTB |= 0b00000000;

    // Incia o LCD
    lcdBegin();

    for (uint8_t byte_idx = 0; byte_idx < strlen(message); byte_idx++) {
        char tx_byte = message[byte_idx];

        for (uint8_t bit_idx = 0; bit_idx < 8; bit_idx++) {
            bool tx_bit = tx_byte & (0x80 >> bit_idx);    

            if(tx_bit)
                PORTB |= 0b00000010;
            else
                PORTB &= 0b01111101;

            _delay_ms((1000 / TX_RATE) / 2);
            // myDelay((1000 / TX_RATE) / 2);

            // Pulsa o clock
            PORTB |= 0b00000001;
            _delay_ms((1000 / TX_RATE) / 2);
            // myDelay((1000 / TX_RATE) / 2);
            PORTB &= 0b01111110;
        }
    }

    // Encerra a transmissão
    PORTB &= 0b01111100; 

    return 0;
}
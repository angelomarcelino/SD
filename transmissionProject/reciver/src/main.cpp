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

#define F_CPU 16000000UL 
#define BAUD 38400
#define MY_UBRR F_CPU/16/BAUD-1

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

void setupSerial(unsigned int ubrr) {
	// Define o baud rate 
    // UBRR0H: contém os 4 bits mais significativos do baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	// UBRR0L: contém os 8 bits menos significatios
    UBRR0L = (unsigned char)ubrr;
	
    // Habilita a transmissão
	UCSR0B = (1<<TXEN0);
	
    // Define o formato dos dados 
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void serialTransmit(unsigned char data) {
    // Espera para a transmissão do buffer
	while ( !( UCSR0A & (1<<UDRE0)) );
	
    // Coloca os dados no buffer de envio
	UDR0 = data;
}

void serialPrint(const char *s) {
	unsigned int i = 0;

	while (s[i]) serialTransmit(s[i++]);
}

int main(void) {

    setupSerial(MY_UBRR);

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

            serialPrint(message);
            serialTransmit('\n');

            lcd.setCursor(0, 1);
            for (uint8_t i = 0; i < 8; i++) {
                if (i < bit_position) {
                    char transmit = rx_byte & (0x80 >> i) ? '1' : '0';
                
                    lcd.print(transmit);
                
                } else 
                    lcd.print(" ");
                
            }

            lcd.setCursor(strlen(message), 0);
            lcd.cursor();
        }
    }

    return 0;
}

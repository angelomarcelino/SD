#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL 
#define BAUD 38400
#define MY_UBRR F_CPU/16/BAUD-1
#define uint unsigned int

bool alert = false;

uint map(uint x, uint in_min, uint in_max, uint out_min, uint out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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

void setupTimer() {
    OCR1A = 0x3D08;

    // CTC no OCR1A
    TCCR1B |= (1 << WGM12);
    // Define interupção por comparação de match
    TIMSK1 |= (1 << OCIE1A);
    // Prescaler de 1024 e start do timer
    TCCR1B |= (1 << CS12) | (1 << CS10);

    sei();
}

ISR(TIMER1_COMPA_vect) { 
    if (alert) {
        DDRC  |= (1<<PC4);
        PORTC |= (1<<PC4);
        _delay_ms(300);
        PORTC &= ~(1<<PC4);
    }
}

void speedometerMode() {
    ADMUX |= 0b00000001;
	ADCSRA |= 0b01000000;
	while (!(ADCSRA & 0b00010000));

    serialPrint("Velocidade: ")
    serialPrint(ADCH);

    uint leds = map(ADCH, 0, 255, 0, 6);
    for (uint i = 0; i < leds; i++) PORTB |= 1<<leds;
}

int main(void) {
    setupTimer();
	setupSerial(MY_UBRR);

    // Configuração do ADC
	ADMUX |= 0b01100000;  // AVCC como referência; ADLAR = 1; MUX = 0000
	ADCSRA |= 0b10000111; // Enable do ADC, Escalonador de Clock em 128;

    // Configuração das saídas
    DDRB |= 0b00111111;
    PORTB |= 0x0;

    // Configura o botão de entrada com PULL-UP
    DDRD &=  0b1111011;
    PORTD |= 0b0000100;

	while(true) {	
        if (PINC & 0b0000001)
			alert = !alert;
		else 
			speedometerMode();
	}
}
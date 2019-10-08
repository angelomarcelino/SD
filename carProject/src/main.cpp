#include <avr/interrupt.h>
#include <Arduino.h>

ISR(TIMER1_COMPA_VECT) {
	// TODO
}

void blinkRightLight() {
	Serial.println("Pisca direito\n");
}

void blinkLeftLight() {
	Serial.println("Pisca esquerdo\n");	
}

int main(void) {
	Serial.begin(115200);

	uint8_t valor_AD;
	// Configuração do ADC
	ADMUX |= 0b01100000;  // AVCC como referência; ADLAR = 1; MUX = 0000
	ADCSRA |= 0b10000111; // Enable do ADC, Escalonador de Clock em 128;

	// Configuração de input/output
	DDRB  &= 0b11110011; // PB2 e PB3 como entrada
	PORTB |= 0b00001100; // PB2 e PB3 como PULL-UP

	DDRD |= 0b11110000; // PD 4, 5, 6 e 7 como saída 
	PORTD |= 0b00000000; // Nenhum PULL-UP

	//Configuração do Contador
	DDRB   |= 0b00000111;
	TCCR1A |= 0b10000011;
	TCCR1B |= 0b00001001;
	TIMSK1 |= 0b00000010;

	// Habilitação dos interrupts
	// sei();

	while(true) {
		if (!(PINB & 0b00000100))
			blinkRightLight();
			
		if (!(PINB & 0b00001000))
			blinkLeftLight();;
		
		// PORTD |= 0b00010000;
		// PORTD |= 0b00100000;


		// Começa a conversão AD
		ADMUX &= 0b11110000;
		ADCSRA |= 0b01000000;
		while (!(ADCSRA & 0b00010000));
		valor_AD = ADCH;
		
		Serial.print("Valor ADC0: ");
		Serial.println(valor_AD); 

		ADMUX |= 0b00000001;
		ADCSRA |= 0b01000000;
		while (!(ADCSRA & 0b00010000));
		valor_AD = ADCH;
		
		Serial.print("Valor ADC1: ");
		Serial.println(valor_AD);
		Serial.println();

		OCR1A = ADC;
	}
}

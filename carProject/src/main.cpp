#include <avr/interrupt.h>
#include <Arduino.h>

#define F_CPU 16000000UL 
#define BAUD 38400
#define MY_UBRR F_CPU/16/BAUD-1

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

}

void blinkRightLight() {
	serialPrint("Pisca direito\n");
}

void blinkLeftLight() {
	serialPrint("Pisca esquerdo\n");	
}

void modoDesligado() {
	serialPrint("\n\t____Modo Desligado____\n");

	// Zerar contadores 

	// Desligar os LEDs/Farois
	PORTD &= 0b00000000;
	PORTD &= 0b00000000;
}

void modoFarois() {
	serialPrint("\n\t____Modo Farois____\n");
	// Ligar farois 
	PORTD |= 0b00010000;
	PORTD |= 0b00100000;

	// Permite alterar o estado dos piscas
	if (!(PINB & 0b00000100))
		blinkRightLight();
		
	if (!(PINB & 0b00001000))
		blinkLeftLight();

	// Sensor de combustivel
	ADMUX |= 0b00000010;
	ADCSRA |= 0b01000000;
	while (!(ADCSRA & 0b00010000));
	
	serialPrint("\nSensor de combustivel: ");
	serialPrint(ADCH);

}

void modoLigado() {
	serialPrint("\n\t____Modo Ligado____\n");

	// Enviar um sinal com 1s de duração

	// Ler velocidade a cada 100ms
	// Sensor de velocidade
	ADMUX |= 0b00000001;
	ADCSRA |= 0b01000000;
	while (!(ADCSRA & 0b00010000));
	
	serialPrint("\nSensor de velocidade: ");
	serialPrint(ADCH);

	// Ligar farois 
	PORTD |= 0b00010000;
	PORTD |= 0b00100000;

	// Permite alterar o estado dos piscas
	if (!(PINB & 0b00000100))
		blinkRightLight();
		
	if (!(PINB & 0b00001000))
		blinkLeftLight();
}

int main(void) {
	setupSerial();
	setupTimer();

	// Configuração do ADC
	ADMUX |= 0b01100000;  // AVCC como referência; ADLAR = 1; MUX = 0000
	ADCSRA |= 0b10000111; // Enable do ADC, Escalonador de Clock em 128;

	// Configuração de input/output
	DDRB  &= 0b11110011; // PB2 e PB3 como entrada
	PORTB |= 0b00001100; // PB2 e PB3 como PULL-UP

	DDRD |= 0b11110000; // PD 4, 5, 6 e 7 como saída 
	PORTD |= 0b00000000; // Nenhum PULL-UP

	while(true) {
		// Lê a chave de ignição
		ADMUX &= 0b11110000;
		ADCSRA |= 0b01000000;
		while (!(ADCSRA & 0b00010000));
		
		if (ADCH < 50)
			modoDesligado();
		else if (ADCH > 50 and ADCH < 100) 
			modoFarois(); 
		else 
			modoLigado();
	}
}

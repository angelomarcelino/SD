#include <avr/interrupt.h>
#include <Arduino.h>

#define F_CPU 16000000UL

ISR(TIMER1_COMPA_VECT) {
	Serial.println("Interrupt");
}

void blinkRightLight() {
	Serial.println("Pisca direito");
}

void blinkLeftLight() {
	Serial.println("Pisca esquerdo");	
}

void modoDesligado() {
	// Serial.print("\n\t____Modo Desligado____\n");

	// Zerar contadores 

	// Desligar os LEDs/Farois
	PORTD &= 0b00000000;
	PORTD &= 0b00000000;
}

void modoFarois() {
	Serial.print("\n\t____Modo Farois____\n");
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
	
	Serial.print("\nSensor de combustivel: ");
	Serial.println(ADCH);

}

void modoLigado() {
	Serial.print("\n\t____Modo Ligado____\n");

	// Enviar um sinal com 1s de duração

	// Ler velocidade a cada 100ms
	// Sensor de velocidade
	ADMUX |= 0b00000001;
	ADCSRA |= 0b01000000;
	while (!(ADCSRA & 0b00010000));
	
	Serial.print("\nSensor de velocidade: ");
	Serial.println(ADCH);

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
	Serial.begin(115200);
	Serial.println("Ready");

	// Configuração do ADC
	ADMUX |= 0b01100000;  // AVCC como referência; ADLAR = 1; MUX = 0000
	ADCSRA |= 0b10000111; // Enable do ADC, Escalonador de Clock em 128;

	// Configuração de input/output
	DDRB  &= 0b11110011; // PB2 e PB3 como entrada
	PORTB |= 0b00001100; // PB2 e PB3 como PULL-UP

	DDRD |= 0b11110000; // PD 4, 5, 6 e 7 como saída 
	PORTD |= 0b00000000; // Nenhum PULL-UP

	//Configuração do Contador
	//  Timer/Counter Control Register
	// TCCR0A |= 0b10000011;
	// Timer/Counter Register 
	// 	TCCR0B |= 0b00001001;
	// Timer/Counter Interrupt Mask Register
	// TIMSK0 |= 0b00000010;

	//Configuração do Contador
	// DDRB   |= 0b00000100;
	// TCCR1A |= 0b10000011;
	// TCCR1B |= 0b00001011;
	// TIMSK1 |= 0b00000010;

	// Habilitação dos interrupts
	// sei();

	OCR2A = 62;
    TCCR2A |= (1 << WGM21);
    // Set to CTC Mode
    TIMSK2 |= (1 << OCIE2A);
    //Set interrupt on compare match
    TCCR2B |= (1 << CS21);
    // set prescaler to 64 and starts PWM
    // sei();

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

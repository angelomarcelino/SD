#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <util/delay.h>

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

void write4bits(uint8_t value);

void pulseEnable(void);

void print(char arr[]);

void send(uint8_t value, uint8_t mode);

void digitalWrite(uint8_t pin, uint8_t state);

inline void command(uint8_t value) { send(value, 0); }

inline void write(uint8_t value) { send(value, 1); }

void lcdBegin() {
    _delay_ms(50);  // wait min 4.1ms

    DDRD |= 0b11111100;  // DEFINE PINOS DO DISPLAY COMO SAIDA

    write4bits(0x03);  // RS R/W BD7 BD6 BD5 BD4
                       // 0   0   0   0   1   1
    _delay_ms(4.5);    // wait min 4.1ms

    // second try
    write4bits(0x03);
    _delay_ms(4.5);  // wait min 4.1ms

    // third go!
    write4bits(0x03);
    _delay_ms(0.15);

    // finally, set to 4-bit interface
    write4bits(0x02);

    // finally, set # lines, font size, etc.
    command(LCD_FUNCTIONSET | 0x0C);  // 0b001011**

    // turn the display on with no cursor or blinking default

    command(LCD_DISPLAYCONTROL | LCD_DISPLAYON);  // 0b000001100

    // clear it off
    command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
    _delay_ms(2);               // this command takes a long time!

    // set the entry mode
    command(LCD_ENTRYMODESET | LCD_ENTRYLEFT);
}

void write4bits(uint8_t value) {
    PORTD |= (value << 2);

    pulseEnable();
}

void pulseEnable(void) {
    digitalWrite(7, 0);
    _delay_ms(0.001);
    digitalWrite(7, 1);
    _delay_ms(0.001);
    digitalWrite(7, 0);
    _delay_ms(0.1);
}

void digitalWrite(uint8_t pin, uint8_t state) {
    if (pin > 0 && pin < 8) {
        DDRD |= (1 << pin);
        PORTD |= (state << pin);
    }
}

void lcdPrint(char arr[]) {
    uint8_t lenght = strlen(arr);
    for (uint8_t i = 0; i < lenght; i++)
        write(arr[i]);
}

void send(uint8_t value, uint8_t mode) {
    digitalWrite(6, mode);

    write4bits(value >> 4);
    write4bits(value);
}

#endif
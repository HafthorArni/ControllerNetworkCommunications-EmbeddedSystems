#include <avr/io.h>
#include <util/delay.h>
#include "digital_in.h"

Digital_in::Digital_in(int pin) {
    pinMask = (1 << pin);
}

void Digital_in::init() {
    DDRB &= ~pinMask;  // Set pin as input
    PORTB |= pinMask; // Enable the internal pull-up resistor
}

bool Digital_in::is_hi() {
    return PINB & pinMask;  // Return true if the pin is high
}

bool Digital_in::is_lo() {
    return !(PINB & pinMask);  // Return true if the pin is low
}

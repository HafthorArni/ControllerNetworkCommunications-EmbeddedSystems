#include "digital_out.h"
#include <stdint.h> 
#include <avr/io.h>
#include <util/delay.h>

Digital_out::Digital_out(int pin)
{
    pinMask = (1 << pin);
}
void Digital_out::init(){
    DDRB |= pinMask;
}
void Digital_out::set_hi(){
    PORTB |= pinMask;
}
void Digital_out::set_lo(){
    PORTB &= ~pinMask;
}
void Digital_out::toggle()
{
    PINB = pinMask;  
}
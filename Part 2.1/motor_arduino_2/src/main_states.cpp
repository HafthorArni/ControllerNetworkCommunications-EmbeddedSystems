#include "digital_in.h"
#include "digital_out.h"
#include "encoder.h"
#include <util/delay.h>
#include <Arduino.h>
#include "timer_msec.h"
#include "analog_out.h"
#include "PI_controller.h"
#include <States.h>
// #include "P_controller.h"

StateVariables stateVars;
Context *context;

int main(){  

    init();// Initialize Arduino framework
    Serial.begin(9600);  
    _delay_ms(1000);
    context = new Context(new Initialization);
    
    //context->timeout(); //Gera þetta ef að fault kemur
    sei();  // enable interrupts

    while(1) {
        context->do_work();
        stateVars.c = '0';

        _delay_ms(3);
        if (Serial.available()) {
            stateVars.c = Serial.read();
            context->command_go();
        }
    }
    return 0;
}

ISR (INT0_vect){
    stateVars.encoder.update();
}

ISR(TIMER1_COMPA_vect){
    // encoder.update();
    stateVars.motorIN1.pin.set_hi();
    stateVars.led.pin.set_hi();
}

ISR(TIMER1_COMPB_vect){
    // encoder.update();
    stateVars.motorIN1.pin.set_lo(); 
    stateVars.led.pin.set_lo();
}
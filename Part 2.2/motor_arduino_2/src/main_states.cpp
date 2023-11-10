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
    Serial.begin(115200, SERIAL_8N1);    
    _delay_ms(1000);
    context = new Context(new Initialization);
    
    //context->timeout(); //Gera þetta ef að fault kemur
    sei();  // enable interrupts

    while(1) {
        context->do_work();
        stateVars.c = '0';

        _delay_ms(3);
        if (Serial.available()) {
            const size_t MSG_LEN = 8;
            uint8_t msg[MSG_LEN];
            char buffer[8]; 
            Serial.readBytes(msg, MSG_LEN);
            if(msg[0] == 0x01){   // checking if this command is for this arduino?
                if (msg[1] == 0x03){    //am I reading?
                    
                }
                if (msg[1] == 0x06){    //am I writing?
                    uint16_t reg = (msg[2]<<8|msg[3]);
                    if (reg == 0x00){   //am I writing on the motor?
                        uint16_t ref = (msg[4]<<8|msg[5]);
                        if(ref>=0 && ref<=255){          // is it in range?
                            stateVars.ref = ref;
                            Serial.println(msg);        //success, sending the message back to the rpi 
                            context->command_go(); 
                        } 
                    }
                }
            } 
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
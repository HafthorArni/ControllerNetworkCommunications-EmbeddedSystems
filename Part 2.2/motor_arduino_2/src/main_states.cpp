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

uint16_t ModRTU_CRC(uint8_t buf[], int len)
{
    uint16_t crc = 0xFFFF;
    for (int pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos]; // XOR byte into least sig. byte of crc
        for (int i = 8; i != 0; i--) { // Loop over each bit
            if ((crc & 0x0001) != 0) { // If the LSB is set
                crc >>= 1; // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else {// Else LSB is not set
                crc >>= 1; // Just shift right
            }
        }
    }
    // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
    return crc;
}

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
            Serial.readBytes(msg, MSG_LEN);
            if(msg[0] == 0x01){   // checking if this command is for this arduino?
                _delay_ms(100);
                if (msg[1] == 0x03){    //am I reading?
                    
                }
                if (msg[1] == 0x06){    //am I writing?
                    uint16_t reg = (msg[2]<<8|msg[3]);
                    if (reg == 0x00){   //am I writing on the motor?
                        uint16_t ref = (msg[4]<<8|msg[5]);
                        if(ref>=0 && ref<=255){          // is it in range?
                            stateVars.ref = ref;
                            uint16_t myCRC = ModRTU_CRC(msg, 6);
                            uint8_t myCRC1 = (uint8_t)(myCRC >> 8);
                            uint8_t myCRC2 = (uint8_t)(myCRC & 0x00FF);
                            if (msg[6] == myCRC1 && msg[7] == myCRC2){
                                Serial.write(msg,8);        //success, sending the message back to the rpi 
                                context->command_go(); 
                            }
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

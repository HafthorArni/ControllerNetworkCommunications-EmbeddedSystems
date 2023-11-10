#include "digital_in.h"
#include "digital_out.h"
#include "encoder.h"
#include <util/delay.h>
#include <Arduino.h>
#include "timer_msec.h"
#include "analog_out.h"
#include "PI_controller.h"
// #include "P_controller.h"


Timer_msec timer;
Digital_out motorIN2(0); //         pin D8
Analog_out motorIN1(1);  // PWM     pin D9
Encoder encoder(3, 4);   // encoder pin D11 D12
float kp = 0.01;
float ti = 5;
PI_controller controller(kp,ti);
// P_controller controller(kp);



unsigned long lastPrintTime = 0;

int analogPin = A3; 
double ref = 0;
double actual = 0;
double pwmValue = 0;
double u = 0;

int main(){  
    init();// Initialize Arduino framework
    Serial.begin(9600);  
    timer.init(0.1); // ms
    sei();  // enable interrupts

    encoder.init();
    motorIN2.init();
    motorIN1.init(10);  // ms
    motorIN1.set(0);  // duty cycle
    motorIN2.set_lo();
    
    while(1){
        ref = (analogRead(analogPin)/1023.0)*120;
        actual = abs(encoder.speed());
        u = controller.update(ref, actual);
        u = constrain(u, 0.0, 0.999); // Ensure pwmValue is within [0, 1]
        pwmValue = u;
        motorIN1.set(pwmValue);
        if (millis() - lastPrintTime >= 250) {  
            Serial.print("speed: (");
            Serial.print("Ref: ");
            Serial.print(ref);
            Serial.print(" - Act: ");
            Serial.print(actual);
            Serial.print(") [RPM], ");
            Serial.print(" duty cycle: ");
            Serial.print(pwmValue);
            Serial.print(" u: ");
            Serial.println(u);
            lastPrintTime = millis();  
        }
        encoder.update();
        _delay_ms(3);
}
    return 0;
}

ISR (INT0_vect){
    encoder.update();
}

ISR(TIMER1_COMPA_vect){
    // encoder.update();
    motorIN1.pin.set_hi();      
}

ISR(TIMER1_COMPB_vect){
    // encoder.update();
    motorIN1.pin.set_lo(); 
}
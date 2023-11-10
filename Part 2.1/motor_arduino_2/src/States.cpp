#include <States.h>
#include <Arduino.h>

void Initialization::on_do()
{
    //on_entry();
}
void Initialization::on_entry()
{
    Serial.println("Init_entry");
    //stateVars.led.init(1000.0);
    //stateVars.led.set(0.001);
    stateVars.pre = false;
    stateVars.op = true;
    stateVars.flt = false;

    stateVars.encoder.init();
    stateVars.motorIN2.init();
    stateVars.motorIN1.init(10);  // ms

    this->context_->command_go();
    
}
void Initialization::on_exit()
{

}
void Initialization::on_command_go()
{
    Serial.println("Automatically go to PreOperational state");
    this->context_->transition_to(new PreOperational);
}

void Initialization::on_command_stop()
{

}

void Initialization::on_timeout()
{

}
// void on_event1();
// void on_event2();

void PreOperational::on_do()
{
    stateVars.motorIN1.set(0.00001);
    if (stateVars.c == 'o' || stateVars.c == 'r'){
        this->context_->command_go();
    }
    if (stateVars.flt){
        this->context_->command_stop();
    }
    
}
void PreOperational::on_entry()
{
    Serial.println("PreOperational entry");
    Serial.println("Blinking LED at 1 Hz");
    //stateVars.led.set(0.5);
    stateVars.pre = true;
}
void PreOperational::on_exit()
{

}
void PreOperational::on_command_go()
{
    if (stateVars.c == 'o'){
        Serial.println("Going to OP");
        this->context_->transition_to(new Operational);
    } 
    if (stateVars.c == 'r'){
        Serial.println("Going to IN");
        this->context_->transition_to(new Initialization);
    }
    if (stateVars.flt == true) {
        stateVars.pre = true;
        Serial.println("PRE:Stop command detected - Stopping");
        this->context_->transition_to(new Stopped);
    }
    if (stateVars.c == '7') {
        stateVars.kp += 0.001; // increase Kp
        Serial.print("Kp = ");
        Serial.println(stateVars.kp,3);
    } 
    if ((stateVars.c == '1') && (stateVars.kp-0.001 > 0)){
        stateVars.kp -= 0.001; // decrease Kp
        Serial.print("Kp = ");
        Serial.println(stateVars.kp,3);
    }
    if (stateVars.c == '9') {
        stateVars.ti += 0.01; // increase Ti
        Serial.print("Ti = ");
        Serial.println(stateVars.ti,3);
    }
    if ((stateVars.c == '3') && (stateVars.ti-0.01 > 0)) {
        stateVars.ti -= 0.01;// decrease Ti
        Serial.print("Ti = ");
        Serial.println(stateVars.ti,3);
    } 
    if (stateVars.c == 'i') {
        stateVars.do_integration = !stateVars.do_integration;
        if (stateVars.do_integration) {
            Serial.println("PI control selected");
        } else {
            Serial.println("P control selected");
        }
    }
}

void PreOperational::on_command_stop()
{
}

void PreOperational::on_timeout(){
}

void Operational::on_do() 
{       
        if (stateVars.faultPin.is_lo()){
            stateVars.faultCount++;

        } else if (stateVars.faultCount > 0) {
            stateVars.faultCount-=10;
        }

        if (stateVars.faultCount > 50) {
            Serial.println("fault");
            stateVars.faultCount = 0;
            stateVars.flt = true;
        }


    if (stateVars.c == 'p' || stateVars.c == 'r'){
        this->context_->command_go();
    }
    if (stateVars.flt){
        this->context_->command_go();
    }
    stateVars.ref = (analogRead(stateVars.analogPin)/1023.0)*120;
    stateVars.actual = abs(stateVars.encoder.speed());

    if (stateVars.do_integration) {
        stateVars.u = stateVars.PIcontroller.update(stateVars.ref, stateVars.actual);
    } else {
        stateVars.u = stateVars.Pcontroller.update(stateVars.ref, stateVars.actual);
    }

    stateVars.u = constrain(stateVars.u, 0.0, 0.999); // Ensure pwmValue is within [0, 1]
    stateVars.pwmValue = stateVars.u;
    stateVars.motorIN1.set(stateVars.pwmValue);
    if (millis() - stateVars.lastPrintTime >= 250) {  
        Serial.print("speed: (");
        Serial.print("Ref: ");
        Serial.print(stateVars.ref);
        Serial.print(" - Act: ");
        Serial.print(stateVars.actual);
        Serial.print(") [RPM], ");
        Serial.print(" duty cycle: ");
        Serial.println(stateVars.pwmValue);
        stateVars.lastPrintTime = millis();  
    }
    stateVars.encoder.update();
}
void Operational::on_entry() 
{
    Serial.println("Operational entry");
    Serial.println("Turning on LED");
    //stateVars.led.set(0.9999);
    stateVars.pre = false;
    stateVars.flt = false;
    stateVars.timer.init(0.1); // ms
    stateVars.encoder.init();
    stateVars.motorIN2.init();
    stateVars.motorIN1.init(10);  // ms
    stateVars.faultPin.init();
    stateVars.motorIN1.set(0.0001);  // duty cycle
    stateVars.motorIN2.set_lo();
}
void Operational::on_exit() 
{

}
void Operational::on_command_go()
{
    if (stateVars.c == 'p'){
        Serial.println("Going to PRE");
        this->context_->transition_to(new PreOperational);
    } 
    if (stateVars.c == 'r'){
        Serial.println("Going to IN");
        this->context_->transition_to(new Initialization);
    }
    if (stateVars.flt == true) {
        stateVars.pre = false;
        Serial.println("OP:Stop command detected - Stopping");
        this->context_->transition_to(new Stopped);
    }
}

void Operational::on_command_stop()
{
}

void Operational::on_timeout()
{
    stateVars.flt = true;
}

void Stopped::on_do() 
{
    stateVars.motorIN1.set(0.0000);
    if (stateVars.c == 'c'){
        stateVars.flt = false;
        this->context_->command_go();
    }
}
void Stopped::on_entry() 
{
    Serial.println("Stopped! waiting for 'c'");
    stateVars.motorIN1.set(0.0000);
    //stateVars.led.set(0.2);
}
void Stopped::on_exit() 
{
    stateVars.flt = false;
}
void Stopped::on_command_go() 
{
    if (stateVars.pre){
        this->context_->transition_to(new PreOperational);
    } else{
        this->context_->transition_to(new Operational);
    }
}

void Stopped::on_command_stop() 
{
}

void Stopped::on_timeout()
{

}
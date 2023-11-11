#include <Arduino.h>
#include <State.h>
#include <analog_out.h>
bool pre;
bool op;
bool flt;
char c;
Analog_out led(5);

class Initialization : public State {
    public:

    void on_do() override
    {
        on_entry();
    }
    void on_entry() override
    {
        Serial.println("Turning off LED");
        led.init(10000.0);
        led.pin.set_lo();
        Serial.println("Initializing c");
        Serial.println("Setting pre = false");
        pre = false;
        Serial.println("Setting op = true");
        op = true;
        Serial.println("Setting FLT = false");
        flt = false;
        this->context_->command_go();
        
    }
    void on_exit() override
    {

    }
    void on_command_go() override
    {
        Serial.println("IN:op = true - We are operational");
        this->context_->transition_to(new PreOperational);
    }

    void on_command_stop() override
    {
    }

    void on_timeout() override;
    // void on_event1();
    // void on_event2();

};

class PreOperational : public State 
{
    public:
    void on_do() override
    {
        Serial.println("Doing fault check");

        Serial.println("Setting c = '0'");
        c = '0';
        Serial.println("Reading into c from keyboard");
        c = Serial.read();
        if (c == 'o' || c == 'r'){
            this->context_->command_go();
        }
        if (flt){
            this->context_->command_stop();
        }
    }
    void on_entry() override
    {
        Serial.println("Blinking LED at 1 Hz");
        led.set(0.1);
        Serial.println("Setting pre = true");
        pre = true;
    }
    void on_exit() override
    {

    }
    void on_command_go() override
    {
        Serial.println("PRE:Leaving pre");
        if (c == 'o'){
            Serial.println("Going to OP");
            this->context_->transition_to(new Operational);
        } 
        if (c == 'r'){
            Serial.println("Going to IN");
            this->context_->transition_to(new Initialization);
        }
    }

    void on_command_stop() override
    {
        Serial.println("PRE:Stop command detected - Stopping");
        this->context_->transition_to(new Stopped);
    }

    void on_timeout() override{
        flt = true;
    }
    // void on_event1();
    // void on_event2();
};

class Operational : public State 
{
    public:
    void on_do() override
    {
        Serial.println("Doing fault check");

        Serial.println("Setting c = '0'");
        c = '0';
        Serial.println("Reading into c from keyboard");
        c = Serial.read();
        if (c == 'p' || c == 'r'){
            this->context_->command_go();
        }
        if (flt){
            this->context_->command_stop();
        }
    }
    void on_entry() override
    {
        Serial.println("Turning on LED");
        led.pin.set_hi();
        Serial.println("Setting pre = false");
        pre = false;
    }
    void on_exit() override
    {

    }
    void on_command_go() override
    {
        Serial.println("OP:Leaving OP");
        if (c == 'p'){
            Serial.println("Going to PRE");
            this->context_->transition_to(new PreOperational);
        } 
        if (c == 'r'){
            Serial.println("Going to IN");
            this->context_->transition_to(new Initialization);
        }
    }

    void on_command_stop() override
    {
        Serial.println("OP:Stop command detected - Stopping");
        this->context_->transition_to(new Stopped);
    }

    void on_timeout() override{
        flt = true;
    }
    // void on_event1();
    // void on_event2();

};

class Stopped : public State {
    public:
    void on_do() override
    {
        Serial.println("Setting c = '0'");
        c = '0';
        Serial.println("Reading into c from keyboard");
        c = Serial.read();
        if (c == 'c'){
            this->context_->command_go();
        }
    }
    void on_entry() override
    {
        Serial.println("Blinking LED at 2 Hz");
        led.set(0.2);
    }
    void on_exit() override
    {
        Serial.println("Setting flt = false");
        flt = false;
    }
    void on_command_go() override
    {
    }

    void on_command_stop() override
    {
        Serial.println("STO:Continue command detected - Continue");
        if (pre){
            this->context_->transition_to(new PreOperational);
        } else{
            this->context_->transition_to(new Operational);
        }
    }

    void on_timeout() override;
    // void on_event1();
    // void on_event2();
};




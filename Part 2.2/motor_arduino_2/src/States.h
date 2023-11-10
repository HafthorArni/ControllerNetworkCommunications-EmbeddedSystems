#ifndef STATES_H
#define STATES_H

#include <Arduino.h>
#include <State.h>
#include <analog_out.h>
#include <encoder.h>
#include <PI_controller.h>

struct StateVariables {
    bool pre;
    bool op;
    bool flt;
    char c;
    float kp = 0.007;
    float ti = 0.21;
    double ref = 0;
    double actual = 0;
    double pwmValue = 0;
    double u = 0;
    int analogPin = A3; 
    unsigned long lastPrintTime = 0;
    int faultCount = 0;
    bool faultDetected = false;
    bool do_integration = true;
    unsigned long faultStartTime = 0;
    Timer_msec timer;
    Digital_in faultPin;  // flt     pin D3
    Digital_out motorIN2; //         pin D8
    Analog_out motorIN1;  // PWM     pin D9
    Encoder encoder;   // encoder pin D11 D12
    Analog_out led;
    PI_controller PIcontroller;
    P_controller Pcontroller;

    StateVariables() : led(5), motorIN1(1), motorIN2(0), encoder(3,4), PIcontroller(kp, ti), Pcontroller(kp), faultPin(3) {} // Initialize the analog_out in the constructor
};

extern StateVariables stateVars;

class Initialization : public State {
    public:
    

    void on_do();
    void on_entry();
    void on_exit();
    void on_command_go();
    void on_command_stop();
    void on_timeout();

};
class PreOperational : public State {
    public:

    void on_do();
    void on_entry();
    void on_exit();
    void on_command_go();
    void on_command_stop();
    void on_timeout();

};

class Operational : public State {
    public:

    void on_do();
    void on_entry();
    void on_exit();
    void on_command_go();
    void on_command_stop();
    void on_timeout();

};

class Stopped : public State {
    public:

    void on_do();
    void on_entry();
    void on_exit();
    void on_command_go();
    void on_command_stop();
    void on_timeout();

};

#endif
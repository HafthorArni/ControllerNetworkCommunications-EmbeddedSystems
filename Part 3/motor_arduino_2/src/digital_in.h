#ifndef DIGITAL_IN_H
#define DIGITAL_IN_H
#include <avr/io.h>
#include <util/delay.h>

class Digital_in {
    public:
    Digital_in(int pin);
    void init();
    bool is_hi();
    bool is_lo();
    private: 
    uint8_t pinMask;
};


#endif // DIGITAL_IN_H
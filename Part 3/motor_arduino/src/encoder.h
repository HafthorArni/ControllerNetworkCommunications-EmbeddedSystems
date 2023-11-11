#ifndef ENCODER_H
#define ENCODER_H

#include "digital_in.h"
class Encoder {
  public:
    Encoder(int pin1, int pin2);
    void init();
    void update();
    int position();
    float speed(); 
  private:
    Digital_in c1, c2;
    int pos;
    bool last_c1;
    bool last_c2;
    int old_pos;
    unsigned long current_time;
    unsigned long last_time;
    double speed_in_rpm; 
};

#endif // ENCODER_H
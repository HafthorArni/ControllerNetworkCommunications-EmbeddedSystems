#include "encoder.h"
#include "Arduino.h"

Encoder::Encoder(int pin1, int pin2) : c1(pin1), c2(pin2), pos(0), last_c1(false), last_c2(false) {
    old_pos = 0;
    current_time = 0;
    last_time = 0;
    speed_in_rpm = 0;

    DDRD &= ~(1 << DDD2);
    PORTD |= (1 << PORTD2);
    EICRA |= (1 << ISC00);
    EIMSK |= (1 << INT0);
    sei();
}

void Encoder::init() {
  c1.init();
  c2.init();
  old_pos = 0;
  current_time = 0;
  last_time = 0;
  speed_in_rpm = 0;
}

void Encoder::update() {
  current_time = millis();
  bool current_c1 = c1.is_hi();
  bool current_c2 = c2.is_hi();

  if (current_c1 != last_c1) {
    if (current_c1 != current_c2) {
      pos++; //clocwise rotation
    } else {
      pos--; //counterclockwise rotation
    }
  }
  last_c1 = current_c1;

  // Compute speed if more than 100ms has passed
  if (current_time - last_time >= 100) {
    int delta_pos = pos - old_pos;
    speed_in_rpm = ((float)delta_pos / (current_time - last_time))*60000.0/1400.0 ; // Convert to RPM

    // Reset old position and last time
    old_pos = pos;
    last_time = current_time;
  }
}

int Encoder::position() {
  return pos;
}

float Encoder::speed() {
  return speed_in_rpm;
}

#include <Arduino.h>
float smoothedValue = 0.0;
float alpha = 0.1; // determines sensitivity of the exponential smoothing function

int sensorPin = 0; // pin A0

void setup() {
  Serial.begin(115200, SERIAL_8N1);
}

void applyExponentialSmoothing(int sensorPin) { // sensor value is noisy so better use smoothing
  int rawReading = analogRead(sensorPin);
  smoothedValue = alpha * (rawReading - smoothedValue) + smoothedValue;
}

void loop(){
  const size_t MSG_LEN = 2;
  uint8_t msg[MSG_LEN];
  char buffer[100];       // stores the return buffer on each loop 
  applyExponentialSmoothing(sensorPin);
  if (Serial.available()>0){                 // bytes received
    const size_t MSG_LEN = 8;
    uint8_t msg[MSG_LEN];
    char buffer[8]; 
    Serial.readBytes(msg, MSG_LEN);
    if(msg[0] == 0x02){   // checking if this command is for this arduino?
        if (msg[1] == 0x03){    //am I reading?
            uint16_t reg = (msg[2]<<8|msg[3]);
            if (reg == 0x00){   //am I reading from the sensor?
                uint16_t ref = (msg[4]<<8|msg[5]);
                if(ref == 0x01){          // is it in range?
                  // taka hér smoother value of setja það í rétt form inn í msg[4] og msg[5]
                  Serial.println(msg);        //success, sending the message back to the rpi
                } 
            }    
        }
        if (msg[1] == 0x06){    //am I writing?
        }
    } 
   }
  // Serial.println((int)smoothedValue);
  delay(0.1);
}

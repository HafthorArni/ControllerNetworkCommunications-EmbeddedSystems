#include <Arduino.h>
float smoothedValue = 0.0;
float alpha = 0.1; // determines sensitivity of the exponential smoothing function
uint16_t data;

int sensorPin = 0; // pin A0

void setup() {
  Serial.begin(115200, SERIAL_8N1);
}

void applyExponentialSmoothing(int sensorPin) { // sensor value is noisy so better use smoothing
  int rawReading = analogRead(sensorPin);
  smoothedValue = alpha * (rawReading - smoothedValue) + smoothedValue;
}
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

void loop(){
  applyExponentialSmoothing(sensorPin);
  if (Serial.available()>0){                 // bytes received
                  // Serial.println("message sent");
    const size_t MSG_LEN = 8;
    uint8_t msg[MSG_LEN];
    Serial.readBytes(msg, MSG_LEN);
    if(msg[0] == 2){
      _delay_ms(100);
      // uint8_t testdata[] = {0x00, 0x00};   // checking if this command is for this arduino?
      // Serial.write(testdata,2);
        if (msg[1] == 0x03){    //am I reading?
            if (msg[3] == 0){   //am I reading from the sensor?
              _delay_ms(100);
              if(msg[5] == 1){          // rpi only asked for one value?
                  uint16_t myCRC = ModRTU_CRC(msg, 6);
                  uint8_t myCRC1 = (uint8_t)(myCRC >> 8);
                  uint8_t myCRC2 = (uint8_t)(myCRC & 0x00FF);
                if (msg[6] == myCRC1 && msg[7] == myCRC2){
                  data = (uint16_t)(smoothedValue);
                  msg[4] = (uint8_t)(data >> 8);
                  msg[5] = (uint8_t)(data & 0x00FF);
                  myCRC = ModRTU_CRC(msg, 6);
                  myCRC1 = (uint8_t)(myCRC >> 8);
                  myCRC2 = (uint8_t)(myCRC & 0x00FF);
                  msg[6] = myCRC1;
                  msg[7] = myCRC2; 
                  Serial.write(msg,8);       //success, sending the message back to the rpi 
                }
              } 
            }    
        }
        if (msg[1] == 0x06){    //am I writing?
        }
    }
   }
  // Serial.println((int)smoothedValue);
  delay(100);
}


#include <Arduino.h>
float smoothedValue = 0.0;
float alpha = 0.1; // determines sensitivity of the exponential smoothing function

int sensorPin = 0; // pin A0

void setup() {
  Serial.begin(9600, SERIAL_8N1);
}

void applyExponentialSmoothing(int sensorPin) { // sensor value is noisy so better use smoothing
  int rawReading = analogRead(sensorPin);
  smoothedValue = alpha * (rawReading - smoothedValue) + smoothedValue;
}

void loop(){
  String command;
  char buffer[100];       // stores the return buffer on each loop 
  applyExponentialSmoothing(sensorPin);
  if (Serial.available()>0){                 // bytes received
      // command = Serial.readStringUntil('\0'); // C strings end with \0
      // if(command.substring(0,4) == "LED "){   // begins with "LED "?
      //    String intString = command.substring(4, command.length());
      //    int level = intString.toInt();       // extract the int
      //    if(level>=0 && level<=255){          // is it in range?
      //       analogWrite(ledPin, level);       // yes, write out
      //       sprintf(buffer, "Set brightness to %d", level);
      //    }
      //    else{                                // no, error message back
      //       sprintf(buffer, "Error: %d is out of range", level);
      //    } 
      // }                                       // otherwise, unknown cmd
      // else{ sprintf(buffer, "Unknown command: %s", command.c_str()); }
      sprintf(buffer, "SE :Sensor value %d\n", (int)smoothedValue);
      Serial.print(buffer);               // send the buffer to the RPi
   }
  // Serial.println((int)smoothedValue);
  delay(0.1);
}

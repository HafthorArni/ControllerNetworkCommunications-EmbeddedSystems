#include <Arduino.h>

float smoothedValue = 0.0;
float alpha = 0.4; // Sensitivity of the exponential smoothing function
int sensorPin = 0; // Pin A0

void setup() {
  Serial.begin(9600, SERIAL_8N1); // Match the baud rate with Raspberry Pi
}

void applyExponentialSmoothing() {
  int rawReading = analogRead(sensorPin);
  smoothedValue = alpha * (rawReading - smoothedValue) + smoothedValue;
}

void loop() {
  applyExponentialSmoothing();
  char buffer[100];       // stores the return buffer on each loop 
  if (Serial.available()) {
    String command = Serial.readStringUntil('\0'); // Read command until newline character
    // Respond to the "SE" command by sending the smoothed value
    if (command == "SE") {
      sprintf(buffer, "SE :Sensor value %d\n", (int)smoothedValue);
      Serial.print(buffer); 
      return;
    } else {
      // For any other command, send a default response or handle as needed
      Serial.println("Unknown command");
      return;
    }
  }
  //delay(1000); // Small delay to prevent overwhelming the serial buffer
}

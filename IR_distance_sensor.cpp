#include <Arduino.h>
float smoothedValue = 0.0;
float alpha = 0.1; // determines sensitivity of the exponential smoothing function

int sensorPin = 0; // pin A0

void setup() {
  Serial.begin(9600);
}

void applyExponentialSmoothing(int sensorPin) { // sensor value is noisy so better use smoothing
  int rawReading = analogRead(sensorPin);
  smoothedValue = alpha * (rawReading - smoothedValue) + smoothedValue;
}

void loop(){
  applyExponentialSmoothing(sensorPin);
  Serial.println((int)smoothedValue);
  delay(0.1);
}

#include <Arduino.h>

const int numSensors = 1;           // Number of sensors
int transposeAmount = 0;            // Transpose amount in semitones
float alpha = 0.1;                 // Smoothing factor for exponential smoothing
float smoothedValue[numSensors] = {0.0}; // Smoothed sensor readings for 2 sensors

const int decreaseButtonPin = 2;
const int increaseButtonPin = 3;
int lastIncreaseState = LOW;
int lastDecreaseState = LOW;

// Apply exponential smoothing to sensor readings
void applyExponentialSmoothing(int sensorIndex) {
  int rawReading = analogRead(sensorIndex);
  smoothedValue[sensorIndex] = alpha * (rawReading - smoothedValue[sensorIndex]) + smoothedValue[sensorIndex];
}

// Handle button presses to update transposeAmount
void handleButtonPresses() {
  int increaseState = digitalRead(increaseButtonPin);
  int decreaseState = digitalRead(decreaseButtonPin);

  if (increaseState == HIGH && lastIncreaseState == LOW) {
    transposeAmount++;
  }
  if (decreaseState == HIGH && lastDecreaseState == LOW) {
    transposeAmount--;
  }

  lastIncreaseState = increaseState;
  lastDecreaseState = decreaseState;
}

// Convert sensor values to MIDI notes using custom segments
int sensorValueToMidiScaleCustomSegments(int num) {
  if (num >= 0 && num <= 1000) {
    int segments[7][2] = {
      {0, 187},
      {188, 200},
      {201, 323},
      {324, 391},
      {392, 459},
      {460, 527},
      {528, 1000}
    };
    int midiNotes[7] = {60, 62, 63, 65, 67, 70, 72};  // Corresponding MIDI note numbers
    
    for (int i = 0; i < 7; ++i) {
      if (num >= segments[i][0] && num <= segments[i][1]) {
        return midiNotes[i] + transposeAmount;
      }
    }
  }
  return -1;  // Return a special value to indicate an error
}

void setup() {
  Serial.begin(9600);
  pinMode(increaseButtonPin, INPUT_PULLUP);
  pinMode(decreaseButtonPin, INPUT_PULLUP);
}

void loop() {
  // Handle button presses for transposing notes
  handleButtonPresses();

  for (int i = 0; i < numSensors; i++) {
    // Apply exponential smoothing to sensor readings
    applyExponentialSmoothing(i);

    // If the sensor index is 0, convert the smoothed sensor value to a MIDI note
    // if (i == 0) {
    //   int midiNote = sensorValueToMidiScaleCustomSegments((int)smoothedValue[i]);
    //   Serial.print(midiNote);
    //   Serial.print(" ");
    // } else {
      // Otherwise, just print the smoothed sensor value
      if ((int)smoothedValue[i]<160){
        Serial.print(0);
        Serial.print(" ");
      } else {
        Serial.print((int)smoothedValue[i]-150);
        Serial.print(" ");
      }
      
    
  }
  Serial.println();
  delay(0.5);
}


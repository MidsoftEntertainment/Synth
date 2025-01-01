#include "analogWave.h"  // Include the library for analog waveform generation

analogWave wave(DAC);  // Create an instance of the analogWave class, using the DAC pin

const int potPin = A5;
const int plusPin = 7;
const int minusPin = 8;

float baseFreq = 493.883f;  // B4 (unreachable)

float currentFreq;
int previousPlusState;
int previousMinusState;

int octaveOffset;
int minOctaveOffset = -4;
int maxOctaveOffset = 4;

void setup() {
  Serial.begin(115200);  // Initialize serial communication at a baud rate of 115200

  pinMode(plusPin, INPUT_PULLUP);
  pinMode(minusPin, INPUT_PULLUP);

  wave.saw(baseFreq);   // Generate a saw wave with the initial frequency
  wave.amplitude(0.f);  // Set the initial amplitude of the waveform generator to 0.f
  currentFreq = baseFreq;
}

void loop() {
  // Handling potentiometer and DAC

  // Read an analog value from pin A5 and map it to a semitone
  int potValue = analogRead(potPin);
  int note = map(potValue, 0, 1023, 0, 24);

  float freq = baseFreq * pow(2.f, note / 12.f);

  if (fabs(currentFreq - freq) >= 0.001f) {
    wave.freq(freq);  // Set the frequency of the waveform generator to the updated value
    wave.amplitude((note == 0) ? 0.f : 1.f);
    currentFreq = freq;
  }

  // Handling octave increment/decrement buttons

  int plusState = digitalRead(plusPin);
  int minusState = digitalRead(minusPin);

  if (plusState == LOW && previousPlusState == HIGH && octaveOffset < maxOctaveOffset) {
    baseFreq *= 2.f;
    octaveOffset++;

    Serial.println("Incrementing octave");
  }

  if (minusState == LOW && previousMinusState == HIGH && octaveOffset > minOctaveOffset) {
    baseFreq /= 2.f;
    octaveOffset--;

    Serial.println("Decrementing octave");
  }

  previousPlusState = plusState;
  previousMinusState = minusState;

  // Print the updated frequency to the serial monitor
  Serial.println("Frequency is now " + String(currentFreq, 3) + " hz " + "(" + String(potValue) + ")");

  delay(100);  // Delay for 100ms before repeating
}

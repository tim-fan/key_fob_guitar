#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "frequencyToNote.h"
#include "pitchToNote.h"

// GUItool: begin automatically generated code
AudioInputAnalog         adc1;           //xy=391,237
AudioAnalyzeNoteFrequency notefreq;      //xy=678,335
AudioConnection          patchCord3(adc1, notefreq);
// GUItool: end automatically generated code

int lastNote = 0;
const int channel = 1;
bool ledState = true;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  AudioMemory(30);
  notefreq.begin(.15);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }
//  Serial.println("Start");
}

void loop() {
  // put your main code here, to run repeatedly:

    if (notefreq.available()) {
        ledState = !ledState;
        digitalWrite(LED_BUILTIN, ledState);
        float freq = notefreq.read();
        float prob = notefreq.probability();
        //Serial.printf("Note: %3.2f | Probability: %.2f\n", freq, prob);
        Serial.print(digitalRead(LED_BUILTIN));
        Serial.println(ledState);

        int midiNote = searchForNote(freq);
//        midiNote = int(map(midiNote, 30,50,72,84));
//        //skip every second note
//        midiNote = (midiNote /2) * 2;
//        //map harmonics to the target range
//        midiNote = (midiNote % 12) + 72;
        
        usbMIDI.sendNoteOff(lastNote, 0, channel);
        usbMIDI.sendNoteOn(midiNote, 99, channel);
        lastNote = midiNote;
    }
}


int searchForNote(float frequency)
{
  float minimum = abs(frequency - noteFrequency[0]);
  float newMinimum;
  int index = 0;

  /*Search for the nearest frequency that is in the vector*/
  for (int i = 0; i < NUMBER_OF_NOTES - 1; i++)
  {
    newMinimum = abs(frequency - noteFrequency[i]);
    if (newMinimum < minimum)
    {
      minimum = newMinimum;
      index = i;
    }
  }
  int note = notePitch[index];          // Use that index to find the corresponding note in the LUT
  return note;
}

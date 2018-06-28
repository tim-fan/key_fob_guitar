#include <TimerOne.h>
#include <Encoder.h>
#include "QuickStats.h"

const int ledPin = 13;
const int midichannel = 1;

//pluck sensor vars
const int pluckSensorPin = A1;
const int pluckSensorBufferSize = 30; //at 2kHz sampling, this gives window size = 15ms
volatile float pluckSensorVals[pluckSensorBufferSize];
volatile int pluckSensorIndex = pluckSensorBufferSize;
const float pluckSensorLoudnessThreshold = 50;
unsigned long lastPluckTime = 0;
const int pluckDebounceTime = 100; //milliseconds
bool lastPluckState = false; //for rising edge detection


Encoder posEncoder(5, 6);
int encoderCorrection = 0; //used to keep encoder values positive

//int numNotes = 7;
//int notes[] = {24, 26, 28, 29, 31, 33, 35}; //midi codes for c major scale
int noteIndex = 0;
////seven nation army
//int bassNote = 74;
//int numNotes = 39;
//int notes[] = {7,7,10,7,5,3,2,7,7,10,7,5,3,5,3,2,7,7,10,7,5,3,2,-2,-2,-2,-2,-2,-2,-2,-2,0,0,0,0,0,0,0,0}; 

//smoke on the water
int bassNote = 69; //for power chord sf2 (uses range 65-86)
int numNotes = 32;
int notes[] = {10,13,15,10,13,16,15,10,13,15,13,10,10,13,15,10,13,16,15,10,13,15,13,10,8,8,8,8,4,4,4,4}; 

int currentNote = notes[0];

QuickStats stats;

void resetPluckValues() {
  noInterrupts();
  for (int i=0; i<pluckSensorBufferSize; i++){
    pluckSensorVals[i] = 0;
  }  
  interrupts();
}

void readPluckSensor() {
    //update index and read pluck sensor value into buffer
    pluckSensorIndex = (pluckSensorIndex + 1) % pluckSensorBufferSize;
    pluckSensorVals[pluckSensorIndex] = float(analogRead(pluckSensorPin));    
}

bool pluckDetected(){
  //return true if sensor currently detects a pluck

  //compute std.dev of current window.
  //if std.dev of window is above a threshold, return true
  noInterrupts();
  float loudness = stats.stdev((float*) pluckSensorVals, pluckSensorBufferSize);
  //Serial.println(loudness);
  bool plucked = loudness > pluckSensorLoudnessThreshold;
  interrupts();

  //debouncing
  if (plucked){
    lastPluckTime = millis();
  }
  
  return lastPluckTime > millis() - pluckDebounceTime;
}

void readSensors(){
  //called in timer interrupt, used to trigger analog reads of sensors  
  readPluckSensor();
  //Serial.println(String(micros()) + ", " + String(pluckSensorVals[pluckSensorIndex]) + ",");
  //Serial.println(pluckSensorVals[pluckSensorIndex]);
}

int encoderToNote(int encoderVal){
  //convert encoder value to a midi note
  
  int ticksPerNote = 40; 

//  //convert to a scale
//  int noteIndex = max(encoderVal / ticksPerNote, 0);
//  int octaveAddition = (int(noteIndex) / numNotes) * 12; 
//  Serial.println(octaveAddition);
//  return notes[noteIndex % numNotes] + octaveAddition;

//  //alernative - just map directly to midi notes
////  int bassNote = 24; //middle c?
//  int bassNote = 74; //for power chord sf2 (uses range 65-86)
//  return encoderVal / ticksPerNote + bassNote;
  
  //alernative - loop through notes in a song, ignoring encoder
  
  noteIndex = (noteIndex + 1) % numNotes;
  return bassNote + notes[noteIndex];
}



void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  
  resetPluckValues();

  Timer1.initialize(500); //setup for 2kHz analog sensor sampling
  Timer1.attachInterrupt(readSensors);
}

void loop() {  
  delay(1);
  digitalWrite(ledPin, pluckDetected());

  bool pluckState = pluckDetected();
  bool pluckRisingEdge = !lastPluckState && pluckState;
  if (pluckRisingEdge){

      int encoderPos = posEncoder.read();
      int correctedPos = encoderPos + encoderCorrection;
    
      //if encoder value goes negative, update correction value to keep the result positive
      if (correctedPos < 0){
        encoderCorrection -= correctedPos;
        correctedPos = 0;  
      }
      
      int nextNote = encoderToNote(correctedPos);
      usbMIDI.sendNoteOff(currentNote, 0, midichannel);
      currentNote = nextNote;
      usbMIDI.sendNoteOn(currentNote, 99, midichannel);
      Serial.println(String("Current note: ") + String(currentNote));
  }
  lastPluckState = pluckState;



//  int nextNote = encoderToNote(correctedPos);
//  if (nextNote != currentNote){
//    usbMIDI.sendNoteOff(currentNote, 0, midichannel);
//    currentNote = nextNote;
//    usbMIDI.sendNoteOn(currentNote, 99, midichannel);
//    Serial.println(String("Current note: ") + String(currentNote));
//  }
  
}


#include <TimerOne.h>
#include "QuickStats.h"

const int ledPin = 13;
const int midichannel = 1;

//pluck sensor vars
const int pluckSensorPin = A0;
const int pluckSensorBufferSize = 30; //at 2kHz sampling, this gives window size = 15ms
volatile float pluckSensorVals[pluckSensorBufferSize];
volatile int pluckSensorIndex = pluckSensorBufferSize;
const float pluckSensorStdDevThreshold = 2;
unsigned long lastPluckTime = 0;
const int pluckDebounceTime = 50; //milliseconds
bool lastPluckState = false; //for rising edge detection

int pluckCount = 0; //for generating a tune from a single note


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
  bool plucked = stats.stdev((float*) pluckSensorVals, pluckSensorBufferSize) > pluckSensorStdDevThreshold;
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
    switch (pluckCount) {
      case 0:
        usbMIDI.sendNoteOff(25, 0, midichannel);  
        usbMIDI.sendNoteOn(24, 99, midichannel);  
      break;
      default:
        usbMIDI.sendNoteOff(24, 0, midichannel);
        usbMIDI.sendNoteOn(25, 99, midichannel);   // 61 = C#4
    }
    pluckCount = (pluckCount + 1) % 2;
  }
  lastPluckState = pluckState;
}

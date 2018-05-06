#include <RotationEncoder.h>

RotationEncoder encoder(A5, A6, A7);

const int ledPin0 = 12;
const int ledPin1 = 11;
const int ledPin2 = 10;

const int buzzerPin = 6;
const int pushbuttonPin = 3;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  
  pinMode(ledPin0, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(pushbuttonPin, INPUT);
  
  encoder.calibrate();
  //encoder.setCalibration(94, 210, 64);
}

int lastRevs = -1;

void loop() {
  encoder.update();
  //encoder.debugPrint();
  
  int currentRevs = encoder.getRevolutions();
  if (lastRevs != currentRevs){
    Serial.println(currentRevs);
    lastRevs = currentRevs;
    tone(buzzerPin, 500 * pow(1.5, currentRevs-2), 100);
  }

  if (!digitalRead(pushbuttonPin)){
    encoder._tickCount = 0;
  }

  digitalWrite(ledPin0, encoder._notchSensors[0].detectsNotch());
  digitalWrite(ledPin1, encoder._notchSensors[1].detectsNotch());
  digitalWrite(ledPin2, encoder._notchSensors[2].detectsNotch());
}

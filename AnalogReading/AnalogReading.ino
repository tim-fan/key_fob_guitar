#include <NotchSensor.h>

RotationEncoder encoder(A5, A6, A7);

const int ledPinA = 12;
const int ledPinB = 11;
const int ledPinC = 10;

const int buzzerPin = 6;
const int pushbuttonPin = 3;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  
  pinMode(ledPinA, OUTPUT);
  pinMode(ledPinB, OUTPUT);
  pinMode(ledPinC, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(pushbuttonPin, INPUT);
  
  encoder.calibrate();
  //encoder.setCalibration(94, 210, 64);
}

int lastCount = -1;

void loop() {
  encoder.update();
  //Serial.print(String(encoder.getCount()) + "\t");
  //encoder.debugPrint();
  //tone(buzzerPin, 500 * pow(1.5, encoder.getCount()));
  //Serial.println();
  int currentCount = encoder.getCount();
  if (lastCount != currentCount){
    Serial.println(currentCount);
    lastCount = currentCount;
    tone(buzzerPin, 500 * pow(1.5, currentCount-2), 100);
  }

  if (!digitalRead(pushbuttonPin)){
    encoder._revolutionCount = 0;
  }

  digitalWrite(ledPinA, encoder._notchSensorA.detectsNotch());
  digitalWrite(ledPinB, encoder._notchSensorB.detectsNotch());
  digitalWrite(ledPinC, encoder._notchSensorC.detectsNotch());
}

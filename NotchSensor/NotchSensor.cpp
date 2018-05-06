#include "NotchSensor.h"
#include "Arduino.h"
#include "QuickStats.h"

NotchSensor::NotchSensor(int pin)
{
    pinMode(pin, INPUT);
    _pin = pin;
    int offVal = 212;
    int onVal = 370;
    //_threshold = offVal + (onVal - offVal) * 0.3;
    _threshold = 120;
    _currentState = false;
}

int NotchSensor::update(void)
{
    int sensorValue = analogRead(_pin);
    //Serial.print(sensorValue);
    _currentState = sensorValue > _threshold;
    
    return sensorValue;
}


bool NotchSensor::detectsNotch(void)
{
    return _currentState;
}

void NotchSensor::calibrate(void)
{
    Serial.println("Calibrating notch sensor on pin " + String(_pin));
    Serial.println("3");
    delay(300);
    Serial.println("2");
    delay(300);
    Serial.println("1");
    delay(300);
    Serial.println("started");
    
    int nSamples = 150;
    float sampleTime = 3000; //milliseconds
    float sensorVals[nSamples];
    
    for (int i = 0; i < nSamples; i++){
        sensorVals[i] = float(analogRead(_pin));
        //sensorVals[i] = 1.2;
        delay(sampleTime/nSamples);
    }
    QuickStats stats;
    float median = stats.median(sensorVals, nSamples);
    float maximum = stats.maximum(sensorVals, nSamples);
    _threshold = median + (maximum - median) * 0.3;
    
    Serial.println("median:\t" + String(median));
    Serial.println("maximum:\t" + String(maximum));
    Serial.println("threshold:\t" + String(_threshold));
}

void NotchSensor::setCalibration(int threshold){
    _threshold = threshold;
}

RotationEncoder::RotationEncoder(int pinA, int pinB, int pinC)
    : _notchSensorA(pinA)
    , _notchSensorB(pinB)
    , _notchSensorC(pinC)
    , _revolutionCount(0)
    , _currentState(A)
    {
}

void RotationEncoder::update(void){
    _notchSensorA.update();
    _notchSensorB.update();
    _notchSensorC.update();
    
    EncoderState lastState = _currentState;
    
    //update current state
    if (_notchSensorA.detectsNotch()){
        _currentState = A;
    }
    else if (_notchSensorB.detectsNotch()){
        _currentState = B;
    }
    else if (_notchSensorC.detectsNotch()){
        _currentState = C;
    }
    
    //handle state transitions.
    
    //transitions out of A set direction, 
    if (lastState == A){
        if (_currentState == B){
            _currentDirection = forward;
        }
        else if (_currentState == C){
            _currentDirection = backward;
        }
    }
    //transitions into A increment or decrement the revolution count
    else if (_currentState == A){
        if ((lastState == C) && (_currentDirection == forward)) {
            _revolutionCount--;
        }
        else if ((lastState == B) && (_currentDirection == backward)) {
            _revolutionCount++;
        }
    }
    
}

void RotationEncoder::calibrate(void){
    _notchSensorA.calibrate();
    _notchSensorB.calibrate();
    _notchSensorC.calibrate();
}

int RotationEncoder::getCount(void){
    return _revolutionCount;
}

void RotationEncoder::debugPrint(void){
    Serial.println(
        String(_revolutionCount)
        + "\tA: " + String(_notchSensorA.detectsNotch()) + " " + String(_notchSensorA.update()) 
        + "\tB: " + String(_notchSensorB.detectsNotch()) + " " + String(_notchSensorB.update()) 
        + "\tC: " + String(_notchSensorC.detectsNotch()) + " " + String(_notchSensorC.update())
    );
}

void RotationEncoder::setCalibration(int thresholdA, int thresholdB, int thresholdC){
    _notchSensorA.setCalibration(thresholdA);
    _notchSensorB.setCalibration(thresholdB);
    _notchSensorC.setCalibration(thresholdC);
}

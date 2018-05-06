#include "RotationEncoder.h"
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

NotchSensor::NotchSensor(void)
{
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

RotationEncoder::RotationEncoder(int pin0, int pin1, int pin2)
    : _tickCount(0)
    , _currentState(0)
    {
        _notchSensors[0] = NotchSensor(pin0);
        _notchSensors[1] = NotchSensor(pin1);
        _notchSensors[2] = NotchSensor(pin2);
}

void RotationEncoder::update(void){
    
    for (int i = 0; i < 3; i++){
        _notchSensors[i].update();
    }
    
    int lastState = _currentState;
    
    //update currentState
    for (int i = 0; i < 3; i++){
        if (_notchSensors[i].detectsNotch()){
            _currentState = i;
        }
    }
    
    //detect transitions
    int stateDiff = _currentState - lastState;
    
    stateDiff = (stateDiff == -2) ? 1 : stateDiff;
    stateDiff = (stateDiff == 2) ? -1 : stateDiff;

    if (stateDiff != 0){
        Serial.println("state diff: " + String(stateDiff));
    } 
    _tickCount -= stateDiff;
    
}

void RotationEncoder::calibrate(void){
    for (int i = 0; i<3; i++){
        _notchSensors[i].calibrate();
    }
}

int RotationEncoder::getRevolutions(void){
    return _tickCount / 3; //three ticks per revolution
}

void RotationEncoder::debugPrint(void){
    Serial.print(String(_tickCount));
    for (int i =0; i < 3; i++){
        Serial.print("\t" + String(i) + ": " + String(_notchSensors[i].detectsNotch()) + " " + String(_notchSensors[i].update()));
    }
    Serial.println();
}

void RotationEncoder::setCalibration(int threshold0, int threshold1, int threshold2){
    _notchSensors[0].setCalibration(threshold0);
    _notchSensors[1].setCalibration(threshold1);
    _notchSensors[2].setCalibration(threshold2);
}

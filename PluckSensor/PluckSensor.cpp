#include "PluckSensor.h"
#include "Arduino.h"
#include "QuickStats.h"

PluckSensor::PluckSensor(int pin)
{
    pinMode(pin, INPUT);
    _pin = pin;
    int offVal = 212;
    int onVal = 370;
    //_threshold = offVal + (onVal - offVal) * 0.3;
    _threshold = 5;
    _currentState = false;
    _currentVal = analogRead(_pin);
    _alpha = 0.2;
    _restingVal = 481;
}

void PluckSensor::update(void)
{
    int sensorValue = analogRead(_pin);
    //Serial.print(sensorValue);
    _currentVal = sensorValue; // * _alpha + _currentVal * (1 - _alpha);
    _currentState = _currentVal > _threshold;
}


bool PluckSensor::isPlucked(void)
{
    return _currentState;
}

void PluckSensor::calibrate(void)
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
    _threshold = median + (maximum - median) * 0.5;
    
    Serial.println("median:\t" + String(median));
    Serial.println("maximum:\t" + String(maximum));
    Serial.println("threshold:\t" + String(_threshold));
}

void PluckSensor::setCalibration(int threshold){
    _threshold = threshold;
}

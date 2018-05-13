
/*
  PluckSensor.h - Defines a class for tracking the rotations of the
  inner drum of a retractable key-fob holder.
  The encoder makes use of three PluckSensors, which can detect the presence
  or absense of a particular notch on the drum, as it rotates. 
*/
#ifndef PluckSensor_h
#define PluckSensor_h


class PluckSensor
{
  public:
    PluckSensor(int pin);
    void update();
    bool isPlucked(); //returns true if notch is currently detected
    void calibrate();
    void setCalibration(int threshold);
    float _currentVal;
    
  private:
    int _pin;
    int _threshold;
    bool _currentState;
    float _alpha; //parameter for smoothing alpha filter
    int _restingVal; //resting analogRead result for sensor when not plucked
};


#endif //PluckSensor_h



/*
  NotchSensor.h - Class representing one of the three sensors in the 
  * key-fob encoder, which detects the movement of the notch in the draw wheel
*/
#ifndef NotchSensor_h
#define NotchSensor_h

class NotchSensor
{
  public:
    NotchSensor(int pin);
    int update();
    bool detectsNotch(); //returns true if notch is currently detected
    void calibrate();
    void setCalibration(int threshold);
  private:
    int _pin;
    int _threshold;
    bool _currentState;
};

//RotationEncoder. 
//Class to count revolutions of a retractable key-fob holder.
//uses three notch detectors around the perimeter of the draw wheel, which 
//are used to track the rotation of the 'notch', and hence count wheel 
//revolutions
class RotationEncoder
{
    public:
        //construct by supplying the pins to use for the three notch sensors
        RotationEncoder(int pinA, int pinB, int pinC);
        
        NotchSensor _notchSensorA;
        NotchSensor _notchSensorB;
        NotchSensor _notchSensorC;
        int _revolutionCount;
        
        void update();
        void calibrate();        
        int getCount(); 
        void debugPrint();
        void setCalibration(int thresholdA, int thresholdB, int thresholdC);
        

    
    private:
        
        
        //enum to represent encoder state, comprised of the most recent sensor to see the notch
        enum EncoderState{A, B, C};
        enum EncoderDirection{forward, backward};
        EncoderState _currentState;
        EncoderDirection _currentDirection;
};

#endif //NotchSensor_h


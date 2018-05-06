
/*
  RotationEncoder.h - Defines a class for tracking the rotations of the
  inner drum of a retractable key-fob holder.
  The encoder makes use of three NotchSensors, which can detect the presence
  or absense of a particular notch on the drum, as it rotates. 
*/
#ifndef RotationEncoder_h
#define RotationEncoder_h

class NotchSensor
{
  public:
    NotchSensor();
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

class RotationEncoder
{
    public:
        //construct by supplying the pins to use for the three notch sensors
        RotationEncoder(int pin0, int pin1, int pin2);
        void update();
        void calibrate();        
        int getRevolutions(); 
        void debugPrint();
        void setCalibration(int threshold0, int threshold1, int threshold2);
        
        NotchSensor _notchSensors[3];
        int _tickCount;
        

        

    
    private:
        
        
        //enum to represent encoder state, comprised of the most recent sensor to see the notch
        //enum EncoderState{A, B, C};
        //enum EncoderDirection{forward, backward};
        //EncoderState _currentState;
        int _currentState; //index of most recently activated sensor
        //EncoderDirection _currentDirection;
};

#endif //RotationEncoder_h


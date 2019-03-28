#include <Arduino.h>

class Motor
{
    public:
        int minSpeed = 590;

        Motor()
        {
            pinMode(22, OUTPUT); //IN1
            pinMode(23, OUTPUT); //IN2
            pinMode(25, OUTPUT); //IN3
            pinMode(26, OUTPUT); //IN4
            
            ledcSetup(0,20000,10); //IN1
            ledcSetup(1,20000,10); //IN2
            ledcSetup(2,20000,10); //IN3
            ledcSetup(3,20000,10); //IN4

            ledcAttachPin(22,0); //IN1
            ledcAttachPin(23,1); //IN2
            ledcAttachPin(25,2); //IN3
            ledcAttachPin(26,3); //IN4
        }

        void directMotors(int targetLocation, bool objectDetected);
        void SetMotorSpeed(int speedL, int speedR);
        void Stop();

};
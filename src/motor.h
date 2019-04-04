#include <Arduino.h>
#include "RemoteDebug.h" 

class Motor
{
    public:
        Motor()
        {
            pinMode(22, OUTPUT); //Motor IN1
            pinMode(23, OUTPUT); //Motor IN2
            pinMode(25, OUTPUT); //Motor IN3
            pinMode(26, OUTPUT); //Motor IN4
            
            ledcSetup(0,30000,10); //Motor IN1
            ledcSetup(1,30000,10); //Motor IN2
            ledcSetup(2,30000,10); //Motor IN3
            ledcSetup(3,30000,10); //Motor IN4

            ledcAttachPin(22,1); //Motor IN1
            ledcAttachPin(23,0); //Motor IN2
            ledcAttachPin(25,3); //Motor IN3
            ledcAttachPin(26,2); //Motor IN4
        }

        void directMotors(int targetLocation, bool objectDetected);
        void enable();
        void disable();
        void Stop();

    private:
        int minSpeed = 700;

        bool motorEnabled = false;
        bool autoMode = true;
        bool started = false;
        bool targetFound = false;
        bool following = false;

        int maxSpeed = 800; 
        int TurnCounter = 0;
        int Counter = 0;
        int lastLocation = 0;

        void SetMotorSpeed(int speedL, int speedR);
        void backward();
        void dance();
        void getClearOfObject();
        void turn();
        void TurnToObject();
        void forward();
};
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
        
        bool autoMode = true;
        bool motorEnabled = false;
        bool started = false;
        bool targetFound = false;
        bool following = false;

        int minSpeed = 700;
        int maxSpeed = 900; 
        int TurnCounter = 0;
        int Counter = 0;
        int lastLocation = 0;

        unsigned long startMillis;  //some global variables available anywhere in the program
        unsigned long currentMillis;
        const unsigned long period = 2000;  //the value is a number of milliseconds

        void SetMotorSpeed(int speedL, int speedR);
        void backward();
        void dance();
        void getClearOfObject();
        void turn();
        void TurnToObject();
        void forward();
};
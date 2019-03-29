#include <Arduino.h>

class Motor
{
    public:
        int minSpeedR = 650;
        int minSpeedL = 700;

        bool motorEnabled = false;
        bool autoMode = true;

        int maxSpeed = 800; //800
        int counter = 0;
        int lastLocation = 0;


        Motor()
        {
            pinMode(22, OUTPUT); //IN1
            pinMode(23, OUTPUT); //IN2
            pinMode(25, OUTPUT); //IN3
            pinMode(26, OUTPUT); //IN4
            
            ledcSetup(0,30000,10); //IN1
            ledcSetup(1,30000,10); //IN2
            ledcSetup(2,30000,10); //IN3
            ledcSetup(3,30000,10); //IN4

            ledcAttachPin(22,1); //IN1
            ledcAttachPin(23,0); //IN2
            ledcAttachPin(25,3); //IN3
            ledcAttachPin(26,2); //IN4
        }

        void directMotors(int targetLocation, bool objectDetected);
        void SetMotorSpeed(int speedL, int speedR);
        void Stop();
        void backward();
        void targetFound();
        void getClearOfObject();
        void turn();
};
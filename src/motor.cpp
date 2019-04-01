#include <Arduino.h>
#include <Wire.h>
#include "motor.h"

void Motor::directMotors(int targetLocation, bool objectDetected) 
{
    if(started)
    {  
        if(objectDetected) //detect object
        {
            motorEnabled = false;
            if(targetLocation < 50 && targetLocation > -50)
            {   
                if(autoMode)
                {    
                    dance();
                }    
            }
            else
            {
                if(autoMode)
                {
                    getClearOfObject();
                }
            } 
        }
        else
        {
            motorEnabled = true;
        } 

        if(autoMode && targetLocation < 176 && targetLocation > -176 && targetLocation != 0 && motorEnabled) //if target in sight
        {
            following = true;
            TurnCounter = 0;
            Counter = 0;
            
            int speedL = map(targetLocation ,-176, 176, maxSpeed, minSpeedR); //map location to speed of motors
            int speedR = map(targetLocation ,-176, 176, minSpeedL, maxSpeed);

            // if(targetLocation < 10 && targetLocation > -10) //set deadband
            // {

            //     speedR = (maxSpeed - minSpeedR)/2 + minSpeedR; 
            //     speedL = (maxSpeed - minSpeedL)/2 + minSpeedL; 
            // }
          
            lastLocation = targetLocation;
            SetMotorSpeed(speedL, speedR);                                
        }
        else
        {
            if(Counter > 5) //target lost
            {        
                if(TurnCounter < 5) //limit turn time
                {
                    TurnToObject();
                    delay(200);
                    TurnCounter++;
                }
                else      //searchmode
                {          
                    int speedR = (maxSpeed - minSpeedR)/2 + minSpeedR; 
                    int speedL = (maxSpeed - minSpeedL)/2 + minSpeedL;

                    SetMotorSpeed(speedL, speedR);                                
                } 
            } 
            Counter++; 
            delay(50);                        
        }  
    }
}

void Motor::SetMotorSpeed(int speedL, int speedR) 
{
    ledcWrite(0, speedL); //IN1
    ledcWrite(1, 0); //IN2

    ledcWrite(2, speedR); //IN3
    ledcWrite(3, 0); //IN4
}

void Motor::Stop() {
    ledcWrite(0, 0); //IN1
    ledcWrite(1, 0); //IN2
    ledcWrite(2, 0); //IN3
    ledcWrite(3, 0); //IN4
}

void Motor::forward()
{
    SetMotorSpeed(maxSpeed, maxSpeed); 
}

void Motor::backward()
{
    ledcWrite(0, 0); //IN1
    ledcWrite(1, minSpeedL); //IN2

    ledcWrite(2, 0); //IN3
    ledcWrite(3, minSpeedR); //IN4  
}

void Motor::dance()
{
    ledcWrite(0, 0); //IN1
    ledcWrite(1, maxSpeed); //IN2

    ledcWrite(2, maxSpeed); //IN3
    ledcWrite(3, 0); //IN4  
    delay(1000);
    Stop();
}

void Motor::turn()
{
    ledcWrite(0, 0); //IN1
    ledcWrite(1, maxSpeed); //IN2

    ledcWrite(2, 0); //IN3
    ledcWrite(3, maxSpeed); //IN4  
}

void Motor::TurnToObject()
{
    if (lastLocation > 0 && motorEnabled)
    {
        SetMotorSpeed(0, minSpeedR); 
    }
    else if (lastLocation < 0 && motorEnabled)
    {
        SetMotorSpeed(minSpeedL, 0); 
    }
}

void Motor::getClearOfObject()
{
    autoMode = false;
    backward();
    delay(500);
    Stop();
    turn();
    delay(1000);
    Stop();
    forward();
    autoMode = true;
}

void Motor::enable()
{
    started = true;
}

void Motor::disable()
{
    started = false;
}
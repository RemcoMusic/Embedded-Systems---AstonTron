#include <Arduino.h>
#include <Wire.h>
#include "motor.h"

void Motor::directMotors(int targetLocation, bool objectDetected) 
{
    if(started)
    { 
        if(objectDetected)  //Detect if there is an object in front of the Robot
        {
            motorEnabled = false;
            if(targetLocation < 100 && targetLocation > -100) //Detect of the object is the target object
            {   
                if(autoMode)
                {    
                    dance();    //Victory dance
                }    
            } 
            else if(autoMode) 
            {
                getClearOfObject(); //Get clear of the object in front of the robot
            }
        }
        else
        {
            motorEnabled = true;
        }

        if(autoMode && (targetLocation < 176) && (targetLocation > -176) && (targetLocation != 0) && motorEnabled) //If target is detected and target is in FOV
        {
            following = true;
            TurnCounter = 0;
            Counter = 0;
            
            int speedL = map(targetLocation ,-176, 176, maxSpeed, minSpeed); //Map the speed to the target location
            int speedR = map(targetLocation ,-176, 176, minSpeed, maxSpeed);
          
            lastLocation = targetLocation;
            SetMotorSpeed(speedL, speedR);                                
        }
        else
        {
            if(Counter > 5) //If the target is lost within the FOV
            {        
                if(TurnCounter < 5) //How long the robot can turn to search for the target
                {
                    TurnToObject();
                    delay(200);
                    TurnCounter++;
                }
                else  //Search mode for the robot
                {          
                    int speedR = (maxSpeed - minSpeed)/2 + minSpeed; 
                    int speedL = (maxSpeed - minSpeed)/2 + minSpeed;

                    SetMotorSpeed(speedL, speedR);                                
                } 
            } 
            Counter++; 
            delay(50);  //delay for target counter                   
        }  
    }
    else
    {
        Stop(); //if motor is not started, Stop
    }
}

void Motor::SetMotorSpeed(int speedL, int speedR) 
{
    ledcWrite(0, speedL);
    ledcWrite(1, 0);

    ledcWrite(2, speedR);
    ledcWrite(3, 0);  
}

void Motor::Stop() {
    ledcWrite(0, 0);
    ledcWrite(1, 0);
    ledcWrite(2, 0);
    ledcWrite(3, 0);
}

void Motor::forward()
{
    SetMotorSpeed(maxSpeed, maxSpeed); 
}

void Motor::backward()
{
    ledcWrite(0, 0);
    ledcWrite(1, minSpeed);

    ledcWrite(2, 0);
    ledcWrite(3, minSpeed);  
}

void Motor::turn()
{
    int direction(random(0,2)); //random value 0 or 1
    if(direction == 0) //turn right
    {
        ledcWrite(0, 0);
        ledcWrite(1, minSpeed);

        ledcWrite(2, minSpeed);
        ledcWrite(3, 0);
    }
    else //turn left
    {
        ledcWrite(0, minSpeed);
        ledcWrite(1, 0);

        ledcWrite(2, 0);
        ledcWrite(3, minSpeed);  
    }
}

void Motor::dance()
{
    started = false;
    backward();
    delay(200);
    Stop();

    for(int i; i < 10;i++)
    {
    ledcWrite(0, 0);
    ledcWrite(1, random(950,1024));
    ledcWrite(2, random(950,1024));
    ledcWrite(3, 0); 
    delay(random(50,200));
    ledcWrite(0, random(950,1024));
    ledcWrite(1, 0);
    ledcWrite(2, 0);
    ledcWrite(3, random(950,1024)); 
    delay(random(50,200));
    }
    Stop();
    delay(3000);
    started = true; //search
}

void Motor::TurnToObject()
{
    if (lastLocation > 0 && motorEnabled) //Decides if the target left the FOV left or right
    {
        SetMotorSpeed(0, minSpeed); 
    }
    else if (lastLocation < 0 && motorEnabled)
    {
        SetMotorSpeed(minSpeed, 0); 
    }
}

void Motor::getClearOfObject()
{
    autoMode = false;
    backward();
    delay(500);
    turn();
    delay(random(300, 600));
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
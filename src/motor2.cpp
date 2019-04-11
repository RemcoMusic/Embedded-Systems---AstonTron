#include <Arduino.h>
#include <Wire.h>
#include "motor2.h"
 

void Motor::directMotors(int targetLocation, bool objectDetected) 
{
    if(started)
    {
     
        currentMillis = millis();  //get the current "time"
   
        if((targetLocation < 176) && (targetLocation > -176) && (targetLocation != 0))
        {
            targetInSight = true;
        }
        else
        {
            targetInSight = false;
        }

        if(targetInSight)
        {
            if(objectDetected && (targetLocation < 100) && (targetLocation > -100) && (targetLocation != 0))
            {
                dance();
            }
            else
            {
                int speedL = map(targetLocation ,-176, 176, maxSpeed, minSpeed); //Map the speed to the target location
                int speedR = map(targetLocation ,-176, 176, minSpeed, maxSpeed);

                lastLocation = targetLocation; //set last location to determine turn direction

                resetCounter();   

                getClearOfObjectActive = false;
                turnDisabled = false;

                SetMotorSpeed(speedL, speedR);   
            }
        }
        else
        {
            if(!objectDetected)
            {
                if((currentMillis - startMillis > 2500))
                {
                    forward();
                }
                else if (!turnDisabled)
                {
                    TurnToObject();
                }        
            }
            else
            {
                if(!getClearOfObjectActive)
                {
                    getClearOfObject();
                }      
            }
        }
    }
}

void Motor::startTimer()
{
    startMillis = millis(); 
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

void Motor::turn(int direction)
{
    if(direction == 1) //Turn right
    {
        ledcWrite(0, 0);
        ledcWrite(1, minSpeed);

        ledcWrite(2, minSpeed);
        ledcWrite(3, 0);
    }
    else //Turn left
    {
        ledcWrite(0, minSpeed);
        ledcWrite(1, 0);

        ledcWrite(2, 0);
        ledcWrite(3, minSpeed);  
    }
}

void Motor::dance()
{
    disable();
    backward();
    delay(200);
    Stop();

    for(int i = 0; i < 10;i++)
    {
    ledcWrite(0, 0);
    ledcWrite(1, random(900,1024));
    ledcWrite(2, random(900,1024));
    ledcWrite(3, 0); 
    delay(random(50,200));
    ledcWrite(0, random(900,1024));
    ledcWrite(1, 0);
    ledcWrite(2, 0);
    ledcWrite(3, random(900,1024)); 
    delay(random(50,200));
    }
    Stop();
}

void Motor::TurnToObject()
{
    turnDisabled = true;
    if (lastLocation > 0 && motorEnabled) //Decides if the target left the FOV left or right
    {
        turn(0);
    }
    else if (lastLocation < 0 && motorEnabled)
    { 
        turn(1);
    }
}

void Motor::getClearOfObject()
{
    getClearOfObjectActive = true;

    backward();
    delay(random(400,600));
    turn(random(0,2)); 	

    int currentCounterTime = millis();
    int counterStart = millis();

    int turntime = random(300, 600);


    while(currentCounterTime - counterStart < turntime)
    {   
        if(!targetFound)
        {
            currentCounterTime = millis();
        }
        else
        {
            forward();
            resetCounter();
            getClearOfObjectActive = false;
            break;
        }   
    }

    forward();
    resetCounter();
    getClearOfObjectActive = false;
}

void Motor::enable()
{
    started = true;
}

void Motor::disable()
{
    started = false;
}

void Motor::resetCounter()
{
    startMillis = currentMillis;  //reset millis counter 
}
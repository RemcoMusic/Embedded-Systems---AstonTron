#include <Arduino.h>
#include <Wire.h>
#include "motor2.h"
 

void Motor::directMotors(int targetLocation, bool objectDetected) 
{
    if(started)
    {
        if(!counterActive)
        {
            currentMillis = millis();  //get the current "time"
            counterActive = true;
        }

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
            if(objectDetected)
            {
                dance();
            }
            else
            {
                int speedL = map(targetLocation ,-176, 176, maxSpeed, minSpeed); //Map the speed to the target location
                int speedR = map(targetLocation ,-176, 176, minSpeed, maxSpeed);

                lastLocation = targetLocation;
                startMillis = currentMillis;  //reset millis counter 
                counterActive = false;     

                SetMotorSpeed(speedL, speedR);   
            }
        }
        else if (!targetInSight)
        {  
            if(objectDetected)
            {
                turn();
            }
            else if (currentMillis - startMillis <= 4000 && !objectDetected)  //test whether the period has elapsed
                { 
                    TurnToObject();
                }
                else
                {
                    forward();
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
}

void Motor::TurnToObject()
{
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
    // uint32_t period = 500L;

    // backward();

    // autoMode = false;
    // for(uint32_t tStart = millis();  (millis()-tStart) < period;)
    // {
    //     autoMode = true;
    //     delay(100);
    //     autoMode = false;
    // }
    // uint32_t period2 = random(600L,1200L);

    // turn(random(0,2));

    // for(uint32_t tStart2 = millis();  (millis()-tStart2) < period2;)
    // {
    //     autoMode = true;
    //     delay(100);
    //     autoMode = false;
    // }
    // forward();
    // autoMode = true;
    backward();
    delay(500);
    turn(random(0,2));
    delay(random(300,1500));
    forward();   
}

void Motor::enable()
{
    started = true;
}

void Motor::disable()
{
    started = false;
}
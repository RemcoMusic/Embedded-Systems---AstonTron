#include <Arduino.h>
#include <Wire.h>
#include "motor.h"

void Motor::directMotors(int targetLocation, bool objectDetected) 
{
    if(autoMode && targetLocation < 176 && targetLocation > -176)
    {
        int speedL = map(targetLocation ,-176, 176, maxSpeed, minSpeedR);
        int speedR = map(targetLocation ,-176, 176, minSpeedL, maxSpeed);

        if(speedR < 40 && speedR > -40)
        {
            speedR = (maxSpeed - minSpeedR)/2 + minSpeedR; 
        }
        if(speedL < 40 && speedL > -40)
        {
            speedL = (maxSpeed - minSpeedL)/2 + minSpeedL; 
        }

        if(objectDetected)
        {
            motorEnabled = false;
            Stop();

        }
        else
        {
            motorEnabled = true;
        }
        
        if(targetLocation != 9999 && targetLocation != 0 && motorEnabled)
        {
            lastLocation = targetLocation;
            SetMotorSpeed(speedL, speedR); 
            counter = 0;
        }
        else
        {
            if(targetLocation < 50 && targetLocation > -50 && !motorEnabled)
            {       
                targetFound();    
            }
            // else
            // {
            //     Stop();  
            //     backward();
            //     delay(1000);        
            //     SetMotorSpeed(minSpeedL, 0); 
            //     delay(1000);
            //     SetMotorSpeed(minSpeedL, 0); 
            //     SetMotorSpeed(0, minSpeedR); 
            // }
        
            counter++;
            if(counter > 3)
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

void Motor::backward()
{
    ledcWrite(0, 0); //IN1
    ledcWrite(1, minSpeedL); //IN2

    ledcWrite(2, 0); //IN3
    ledcWrite(3, minSpeedR); //IN4  
}

void Motor::targetFound()
{
    ledcWrite(0, 0); //IN1
    ledcWrite(1, maxSpeed); //IN2

    ledcWrite(2, maxSpeed); //IN3
    ledcWrite(3, 0); //IN4  

    delay(2000);

    ledcWrite(0, 0); //IN1
    ledcWrite(1, maxSpeed); //IN2

    ledcWrite(2, 0); //IN3
    ledcWrite(3, maxSpeed); //IN4  

    Stop();
}

void Motor::turn()
{
    ledcWrite(0, 0); //IN1
    ledcWrite(1, maxSpeed); //IN2

    ledcWrite(2, 0); //IN3
    ledcWrite(3, maxSpeed); //IN4  
}

void Motor::getClearOfObject()
{
    autoMode = false;
    backward();
    delay(1000);
    turn();

    
}
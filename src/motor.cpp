#include <Arduino.h>
#include "motor.h"

void Motor::Forward()
{
    int speed = 100;
    int minSpeed = 500;

    speed = map(speed, 0, 100, minSpeed, 1024);
 
    for(int x = minSpeed; x <= speed; x = x + 5)
    {
    ledcWrite(0, 0); //IN1
    ledcWrite(1, x); //IN2

    ledcWrite(2, x); //IN3
    ledcWrite(3, 0); //IN4

    delay(20);
    }
}

void Motor::Right()
{
 for(int x = 400; x <= 800; x = x + 5)
    {
    ledcWrite(0, x); //IN1
    ledcWrite(1, 0); //IN2

    ledcWrite(2, x); //IN3
    ledcWrite(3, 0); //IN4
    delay(20);
    }
 Stop(); 
}

void Motor::Left()
{
 for(int x = 400; x <= 800; x = x + 5)
    {
    ledcWrite(0, 0); //IN1
    ledcWrite(1, x); //IN2

    ledcWrite(2, 0); //IN3
    ledcWrite(3, x); //IN4
    delay(20);
    }
 Stop(); 
}

void Motor::Stop()
{
    ledcWrite(0, 0); //IN1
    ledcWrite(1, 0); //IN2
    ledcWrite(2, 0); //IN3
    ledcWrite(3, 0); //IN4
}
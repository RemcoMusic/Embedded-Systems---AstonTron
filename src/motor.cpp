#include <Arduino.h>
#include <Wire.h>
#include "motor.h"

void Motor::directMotors(int targetLocation, bool objectDetected) {

    int speedL = map(targetLocation ,-176, 0, 0, 100);
    int speedR = map(targetLocation ,0, 176, 0, 100);

    Serial.print("Location: ");
    Serial.println(targetLocation);
    Serial.print("SpeedL: ");
    Serial.println(speedL);
    Serial.print("SpeedR: ");
    Serial.println(speedR);

    if(targetLocation != 9999 || objectDetected == false)
    {
        //SetMotorSpeed(speedL, speedR);
    }
    else
    {
        Stop();
    }

    // if (targetLocation < -30 && targetLocation > -60) {
    //     SetMotorSpeed(80,80);
    // }else if (targetLocation > 60 && targetLocation < 90) {
    //     SetMotorSpeed(0,50);
    // }else if (targetLocation < -120 && targetLocation > -170) {
    //     SetMotorSpeed(50,0);
    // }else if (targetLocation == 9999 || objectDetected == true){
    //     Stop();
    // }  
    delay(200);
}

void Motor::SetMotorSpeed(int speedL, int speedR) {
    speedL = map(speedL, 0, 100, 0, 1024);
    speedR = map(speedR, 0, 100, 0, 1024);

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
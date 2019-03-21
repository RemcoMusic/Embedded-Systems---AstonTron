#include <Arduino.h>
#include "distanceSensor.h"


void DistanceSensor::beginSensor()
{
    distanceSensor.init();
    distanceSensor.setSignalRateLimit(SignalRate);
    distanceSensor.setTimeout(500);
    distanceSensor.setMeasurementTimingBudget(200000);
}

int DistanceSensor::readDistanceSensor()
{
    int CurrentDistance = distanceSensor.readRangeSingleMillimeters(); 
  
    if(CurrentDistance <= 1000) //If distance larger that 1m send 9999
    {
       return CurrentDistance;  
    }
}

int DistanceSensor::detectObjects()
{
    int correctedDistance = readDistanceSensor();

    if(correctedDistance != 0)
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}
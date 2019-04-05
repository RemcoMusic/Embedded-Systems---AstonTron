#include <Arduino.h>
#include "distanceSensor.h"


void DistanceSensor::beginSetup()
{
    distanceSensor.setSignalRateLimit(SignalRate);
    distanceSensor.setTimeout(500);
    distanceSensor.setMeasurementTimingBudget(200000);
    distanceSensor.init();
}

int DistanceSensor::readDistanceSensor()
{
    int CurrentDistance = distanceSensor.readRangeSingleMillimeters(); 
  
    if(CurrentDistance <= 1500)
    {
       return CurrentDistance;
    }
    else
    {
        return 9999;
    }
    
}
#include <Arduino.h>
#include <MechaQMC5883.h>
#include "compass.h"

MechaQMC5883 compassSensor;

void Compass::beginSetup()
{
    compassSensor.init();
}

int Compass::readCompass()
{
    int x, y, z;
    compassSensor.read(&x, &y, &z);
    return compassSensor.azimuth(&x,&z);
    delay(100);
}
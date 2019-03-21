#include <Arduino.h>
#include <VL53L0X.h>

class DistanceSensor
{
    public:
        void beginSensor();
        int readDistanceSensor();
        int detectObjects();

        VL53L0X distanceSensor;
        float SignalRate = 0.25;
};
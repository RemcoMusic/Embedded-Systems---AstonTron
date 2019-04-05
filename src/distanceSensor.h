#include <Arduino.h>
#include <VL53L0X.h>

class DistanceSensor
{
    private:
        VL53L0X distanceSensor;
        float SignalRate = 0.25;

    public:
        void beginSetup();
        int readDistanceSensor();  
};
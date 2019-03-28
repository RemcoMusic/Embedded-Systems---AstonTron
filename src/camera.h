#include <Arduino.h>

class Camera
{
    private:
        int objectLocation;

    public:
        void beginSetup();
        void readCamera();
        
        void setObjectLocation(int);
        int getObjectLocation();

};
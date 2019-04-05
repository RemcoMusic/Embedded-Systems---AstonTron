#include <Arduino.h>

class Camera
{
    private:
        int objectLocation;
        int MAX_CMD_LENGTH = 10;
        char cmd[10];
        int cmdIndex;
        char incomingByte;

    public:
        void beginSetup();
        void readCamera();
        
        void setObjectLocation(int);
        int getObjectLocation();

};
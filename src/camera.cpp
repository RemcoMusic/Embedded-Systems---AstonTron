#include <Arduino.h>
#include "camera.h"

HardwareSerial serialComsCamera(2);

void Camera::beginSetup()   //Open the connection to communicate with the camera through the UART protocol
{
    serialComsCamera.begin(115200,SERIAL_8N1,17,16);
    cmdIndex = 0;
    serialComsCamera.write("Begin \n"); //Send first message for comfirmation communication
}

void Camera::readCamera()   //Read the incoming bytes until \n and convert them to an int
{
    if((incomingByte=serialComsCamera.available()>0)) 
    { 
        char byteIn = serialComsCamera.read();
        cmd[cmdIndex] = byteIn;
        
        if(byteIn=='\n') 
        {
            cmd[cmdIndex] = '\0';
            cmdIndex = 0;
            
            if(cmd != 0) 
            {
                serialComsCamera.write("Yes \n");
            }
            int i = atoi(cmd);
            setObjectLocation(i);
        }
        else
        {
            if(cmdIndex++ >= MAX_CMD_LENGTH)
            {
                cmdIndex = 0;
            }
        }
    }
}

void Camera::setObjectLocation(int x) 
{
    objectLocation = x;
}

int Camera::getObjectLocation() 
{
    return objectLocation;
}
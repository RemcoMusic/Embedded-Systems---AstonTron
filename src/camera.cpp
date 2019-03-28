#include <Arduino.h>
#include "camera.h"

HardwareSerial mySerial(2);

int MAX_CMD_LENGTH = 10;
char cmd[10];
int cmdIndex;
char incomingByte;

void Camera::beginSetup()
{
    mySerial.begin(115200,SERIAL_8N1,17,16);
    cmdIndex = 0;
    mySerial.write("Begin \n");
}

void Camera::readCamera()
{
    if (incomingByte=mySerial.available()>0) {
        
        char byteIn = mySerial.read();
        cmd[cmdIndex] = byteIn;
        
        if(byteIn=='\n') {
            cmd[cmdIndex] = '\0';
            cmdIndex = 0;
            
            if(cmd != 0) {
                mySerial.write('Yes \n');
            }
            int i = atoi(cmd);
            Serial.println(i);
            setObjectLocation(i);
        }else{
            if(cmdIndex++ >= MAX_CMD_LENGTH){
            cmdIndex = 0;
            }
        }
    }
}

void Camera::setObjectLocation(int x) {
    objectLocation = x;
}

int Camera::getObjectLocation() {
    return objectLocation;
}
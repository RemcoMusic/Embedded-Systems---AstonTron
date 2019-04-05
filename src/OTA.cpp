#include <Arduino.h>
#include <ArduinoOTA.h>
#include "OTA.h"

void OTA::startOTA()
{
  ArduinoOTA.setHostname("AstontronOTA");
      
    ArduinoOTA.onStart([]() 
    {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
      {
        type = "sketch";
      }
      else
      {
        type = "filesystem";
      }
    });

    ArduinoOTA.begin();
    while(true)
    {
      ArduinoOTA.handle();
    }
}
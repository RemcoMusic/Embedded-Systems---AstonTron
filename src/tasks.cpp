#include <Arduino.h>
#include <ESPmDNS.h>
#include "RemoteDebug.h" 
#include "OTA.h"
#include "motor.h"
#include "distanceSensor.h"
#include "tasks.h"

RemoteDebug Debug;
OTA OTAUpdate;
Motor motor;
DistanceSensor dSensor;

int objectDetected;
bool motorEnabled = false;
TaskHandle_t xHandle = NULL;

void processCmdRemoteDebug() {

	String lastCmd = Debug.getLastCommand();

	if (lastCmd == "ota") {
      debugI("ota starting");
      OTAUpdate.startOTA();
  }

	if (lastCmd == "test") {
      debugI("test suceeded");
  }

  	if (lastCmd == "start") {
      debugI("Starting driving");
      motorEnabled = true;     
  }
  if (lastCmd == "stop") {
      debugI("Stop gaat fout kut");
      motorEnabled = false;
      motor.Stop();     
  }
  if (lastCmd == "sensor") {
      debugI("sensor");
      dSensor.beginSensor();

      for(int x = 0; x < 10; x++)
      {
      debugI("Sensor value:"); 
      debugI("* Distance: %u mm", dSensor.readDistanceSensor());
      delay(500);
      }

      //vTaskDelete(xHandle);
    
  }
}

Tasks::Tasks()
{
  xTaskCreate(
    remoteDebugger,
    "Debugger",
    10000,
    NULL,
    1,
    NULL);

  xTaskCreate(
    motorDriver,
    "Motor",
    10000,
    NULL,
    2,
    &xHandle);

  xTaskCreate(
    readDistanceSensor,
    "Read_Distance_Sensor",
    10000,
    NULL,
    1,
    NULL);
}

void Tasks::readDistanceSensor(void * parameter)
{
  dSensor.beginSensor();
  for(;;)
  {
    int distance = dSensor.readDistanceSensor();
    debugI("* Distance: %u mm", dSensor.readDistanceSensor());
    if(distance < 100)
    {
      objectDetected = true;
    }
    //objectDetected = dSensor.detectObjects();
    delay(200);
  }
}


void Tasks::motorDriver(void * parameter)
{
  bool driving = false;
  for(;;)
  {
    if(motorEnabled)
    {
      if(objectDetected == 0 && driving == false) {
        motor.Forward();
        driving = true;
      } 
      else if (objectDetected == 1) {
        debugI("Object detected");
        motor.Stop();
        delay(2000);
        motor.Right();
        objectDetected = false;
        delay(2000);
      }
    }
    delay(100);
  }
}

void Tasks::remoteDebugger(void * parameter)
{

    if (MDNS.begin("AstontronDebug")) {
        Serial.print("* MDNS responder started. Hostname -> ");
        Serial.println("AstontronDebug");
    }

    MDNS.addService("telnet", "tcp", 23);

    Debug.begin("AstontronDebug"); // Initiaze the telnet server
    Debug.setResetCmdEnabled(true); // Enable the reset command
	  Debug.showProfiler(false); // Profiler (Good to measure times, to optimize codes)
	  Debug.showColors(true); // Colors
    Debug.setCallBackProjectCmds(&processCmdRemoteDebug);
  
    for(;;)
    {
      Debug.handle();   
    }
}
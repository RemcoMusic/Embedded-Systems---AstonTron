#include <Arduino.h>
#include <ESPmDNS.h>
#include <Wire.h>
#include "RemoteDebug.h" 
#include "OTA.h"
#include "motor.h"
#include "camera.h"
#include "distanceSensor.h"
#include "tasks.h"

RemoteDebug Debug;
OTA OTAUpdate;
Motor motor;
DistanceSensor dSensor;
Camera openMV;

TwoWire I2CdistanceSensor = TwoWire(0);

bool objectDetected = false;
bool motorDriverEnabled = false;

void processCmdRemoteDebug() {

	String lastCmd = Debug.getLastCommand();

	if (lastCmd == "ota") {
      debugI("ota starting");
      OTAUpdate.startOTA();
  }

  if (lastCmd == "start") {
      debugI("Starting driving"); 
      motor.enable();
  }
  if (lastCmd == "stop") {
      debugI("Stop gaat fout kut");
      motor.disable();    
      motor.Stop();
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
}

void Tasks::readDistanceSensor(void * parameter)
{
  I2CdistanceSensor.begin(27,32,100000);
  dSensor.beginSetup();
  for(;;)
  {
    int distance = dSensor.readDistanceSensor();
    if(distance < 120)
    {
      objectDetected = true;
      debugE("Object Detected");
    }
    else {
      objectDetected = false;
    }
    delay(50);
  }
}
void Tasks::cameraInput(void * parameter) {
  openMV.beginSetup();
  for(;;) {
    openMV.readCamera();
  }
}

void Tasks::motorDriver(void * parameter) {
  for(;;) 
    {
      motor.directMotors(openMV.getObjectLocation(), objectDetected);  
    }
}

void Tasks::remoteDebugger(void * parameter) {
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

    xTaskCreate(
      readDistanceSensor,
      "Read_Distance_Sensor",
      10000,
      NULL,
      1,
      NULL);

    xTaskCreate(
      cameraInput,
      "Camera_Input",
      10000,
      NULL,
      1,
      NULL);
    
    xTaskCreate(
      motorDriver,
      "Motor",
      10000,
      NULL,
      1,
      NULL);

    for(;;)
    {
      Debug.handle();   
    }
}
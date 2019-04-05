#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include "tasks.h"

const char* ssid = "Astontron";
const char* password = "Secret123";

void connectWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
}

void setup() 
{
  Serial.begin(115200);
  Serial.println("Booting");
  connectWifi();
  delay(2000);
  disableCore0WDT();
  Tasks initTasks; 
}

void loop() 
{
  delay(500);
}
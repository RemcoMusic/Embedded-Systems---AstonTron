#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include "tasks.h"

const char* ssid = "Astontron";
const char* password = "Secret123";

TwoWire I2CdistanceSensor = TwoWire(0);

void connectWifi(){

 WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  I2CdistanceSensor.begin(27,32,100000);
  connectWifi();
  disableCore0WDT();
  Tasks initTasks; 
}

void loop() {
  delay(500);
}
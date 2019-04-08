#include <Arduino.h>
#include <IRremote.h>
#include "IR.h"

int RECV_PIN = 21;

IRrecv irrecv(RECV_PIN);

decode_results results;

void IR::start()
{
  irrecv.enableIRIn();
}

unsigned long IR::Recieve() {
if (irrecv.decode(&results)) {
    //Serial.println(results.value, HEX);
    unsigned long IRcode = results.value;
    return(IRcode);
  }
}

void IR::Next() {
  irrecv.resume(); 
}

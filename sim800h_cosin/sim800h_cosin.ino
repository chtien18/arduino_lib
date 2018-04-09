// **** INCLUDES *****
#include "LowPower.h"

void setup()
{
  pinMode(13,OUTPUT);
    // No setup is required for this library
}

void loop() 
{
    // Enter power down state for 8 s with ADC and BOD module disabled
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  
    digitalWrite(13,HIGH);
    delay(200);
    digitalWrite(13,LOW);
    // Do something here
    // Example: Read sensor, data logging, data transmission.
}

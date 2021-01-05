#include <SDI12.h>

#define SERIAL_BAUD 115200 /*!< The baud rate for the output serial port */
#define DATA_PIN 8        /*!< The pin of the SDI-12 data bus */
//#define POWER_PIN 22       /*!< The sensor power pin (or -1 if not switching power) */

/** Define the SDI-12 bus */
SDI12 mySDI12(DATA_PIN);

float sensorValues[15] = {0};  // Create an array to receive data

String myCommand = "0R7!";

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println("Opening SDI-12 bus...");
  mySDI12.begin();
  delay(500);  
}

void loop() {
    mySDI12.sendCommand(myCommand);
    delay(1000);                    // wait a while for a response
    Serial.println(F("Receiving data"));  // For debugging
    
    for (int i = 0; i < 15; i++)
    {
      float result = mySDI12.parseFloat();
      sensorValues[i] += result;
      Serial.print(F("Data #"));  // For debugging
      Serial.print(i);  // For debugging
      Serial.print(F(": "));  // For debugging
      Serial.println(result);  // For debugging
    }

    mySDI12.flush();
    delay(300);
    Serial.println();
    delay(10000); 
}

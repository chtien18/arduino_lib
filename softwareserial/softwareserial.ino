#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
 
  while(!Serial);
  Serial.println("demo");

}

void loop() {


 if (Serial.available()) {
      Serial.write(Serial.read());
    }
}


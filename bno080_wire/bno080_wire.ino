#include <Wire.h>

#define BNO_ADDRESS 0x4B

void setup() {
   Wire.begin(0x4B);        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

void loop() {
 Wire.requestFrom(BNO_ADDRESS, 6);    // request 6 bytes from slave device #8

  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }
  delay(500);

}

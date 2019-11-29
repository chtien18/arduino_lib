// Set serial for debug console (to the Serial Monitor, speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to the module)
// Use Hardware Serial on Mega, Leonardo, Micro
//#define SerialAT Serial1
// or Software Serial on Uno, Nano
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(11, 13); // RX, TX

void setup() {
  pinMode(7,OUTPUT);
  digitalWrite(7,HIGH);
  delay(5000);
  SerialMon.begin(9600);
  delay(100);
  SerialAT.begin(9600);
  delay(100);
  
}
void loop() {
  // put your main code here, to run repeatedly:
 if (SerialAT.available()) {
      SerialMon.write(SerialAT.read());
    }
 if (SerialMon.available()) {
      SerialAT.write(SerialMon.read());
    }
}

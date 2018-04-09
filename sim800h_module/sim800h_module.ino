

// Set serial for debug console (to the Serial Monitor, speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
// Use Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial2

// or Software Serial on Uno, Nano
//#include <SoftwareSerial.h>
//SoftwareSerial SerialAT(2, 3); // RX, TX


void setup() {
  // put your setup code here, to run once:
  // Set console baud rate
  SerialMon.begin(9600);
  delay(10);
  SerialAT.begin(9600);
  delay(10);
  pinMode(9,OUTPUT);
  digitalWrite(9,LOW);
  delay(2000);
  digitalWrite(9,HIGH);
  delay(13000);
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

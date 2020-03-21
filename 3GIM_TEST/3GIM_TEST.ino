#include <SoftwareSerial.h>
SoftwareSerial Serial3g(4, 5);
const unsigned long baudrate = 9600;
void setup() {
Serial.begin(baudrate);
Serial3g.begin(baudrate);
pinMode(7,OUTPUT);
digitalWrite(7,HIGH); delay(5);
digitalWrite(7,LOW); //3GIMシールド電源ON
Serial.println("Ready.");
}
void loop() {
if (Serial3g.available() > 0) {
char c = Serial3g.read();
Serial.print(c);
}
if (Serial.available() > 0) {
char c = Serial.read();
Serial.print(c); // Echo back
Serial3g.print(c);
}
}

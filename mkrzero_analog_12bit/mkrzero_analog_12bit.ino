/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255 and uses
  the result to set the pulse width modulation (PWM) of an output pin.
  Also prints the results to the Serial Monitor.

  The circuit:
  - potentiometer connected to analog pin 0.
    Center pin of the potentiometer goes to the analog pin.
    side pins of the potentiometer go to +5V and ground
  - LED connected from digital pin 9 to ground

  created 29 Dec. 2008
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogInOutSerial
*/

// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int LED =7;

int sensorValue = 0;        // value read from the pot


void setup() {
  // initialize serial communications at 9600 bps:
  pinMode(LED,OUTPUT);
  pinMode(analogInPin,INPUT);
  Serial.begin(9600);
  analogReadResolution(12);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  

  // print the results to the Serial Monitor:
  Serial.print("sensor = ");
  Serial.println(sensorValue);


  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(500);
  digitalWrite(LED,HIGH);
   sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  

  // print the results to the Serial Monitor:
  Serial.print("sensor = ");
  Serial.println(sensorValue);
  delay(500);
  digitalWrite(LED,LOW);
  
}

/*
  Analog Input
 Demonstrates analog input by reading an analog sensor on analog pin 0 and
 turning on and off a light emitting diode(LED)  connected to digital pin 13. 
 The amount of time the LED will be on and off depends on
 the value obtained by analogRead(). 
 
 The circuit:
 * Potentiometer attached to analog input 0
 * center pin of the potentiometer to the analog pin
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to digital output 13
 * LED cathode (short leg) attached to ground
 
 * Note: because most Arduinos have a built-in LED attached 
 to pin 13 on the board, the LED is optional.
 
 
 Created by David Cuartielles
 modified 30 Aug 2011
 By Tom Igoe
 
 This example code is in the public domain.
 
 http://arduino.cc/en/Tutorial/AnalogInput
 
 */

int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
  // Pin 13: Arduino has an LED connected on pin 13
  // Pin 11: Teensy 2.0 has the LED on pin 11
  // Pin  6: Teensy++ 2.0 has the LED on pin 6
  // Pin 13: Teensy 3.0 has the LED on pin 13
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);  
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);    
  // turn the ledPin on
  //digitalWrite(ledPin, HIGH);  
  // stop the program for <sensorValue> milliseconds:
//  delay(sensorValue);          
  // turn the ledPin off:        
 // digitalWrite(ledPin, LOW);   
  // stop the program for for <sensorValue> milliseconds:
  //delay(sensorValue);      
int d=get_wind_direction();
  Serial.println(d);            
}

//Read the wind direction sensor, return heading in degrees
int get_wind_direction()
{
    unsigned int adc;

    adc = analogRead(A0); // get the current reading from the sensor
    Serial.println(adc);
    Serial.print("angle:");

    // The following table is ADC readings for the wind direction sensor output, sorted from low to high.
    // Each threshold is the midpoint between adjacent headings. The output is degrees for that ADC reading.
    // Note that these are not in compass degree order! See Weather Meters datasheet for more information.

    if (adc < 65) return (113);
    if (adc < 84) return (68);
    if (adc < 92) return (90);
    if (adc < 125) return (158);
    if (adc < 186) return (135);
    if (adc < 245) return (203);
    if (adc < 287) return (180);
    if (adc < 405) return (23);
    if (adc < 465) return (45);
    if (adc < 601) return (248);
    if (adc < 633) return (225);
    if (adc < 707) return (338);
    if (adc < 790) return (0);
    if (adc < 830) return (293);
    if (adc < 890) return (315);
    if (adc < 949) return (270);
    return (-1); // error, disconnected?
}

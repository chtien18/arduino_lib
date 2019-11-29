
#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);
float t;
float h;

String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  inputString.reserve(200);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (stringComplete) {
   // Serial.println(inputString);
    if(inputString=="1\n"){
      digitalWrite(13,HIGH);}
    if(inputString=="0\n"){
      digitalWrite(13,LOW);}  
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
  String data=buildJson();
  Serial.print(data);

  delay(2000);

}

String buildJson() {
  String data = "{";
  data+="\"Humidity\": ";
  data+=(float)h;
  data+= ",";
  data+="\"Temperature\": ";
  data+=(float)t;
  data+="}";
  data+="\n";
  return data;
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

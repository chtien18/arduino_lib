//Arduino Zero
#include "LowPower.h"
#include <SPI.h>
#include <LoRa.h>

int average_value1 = 0;
void setup() {
  pinMode(A0,INPUT);//EC5
  pinMode(A1,INPUT);//batt
  pinMode(4,OUTPUT);
  Serial.begin(9600);
  //while (!Serial);

 // Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
   Serial.println("Starting LoRa failed!");
    while (1);
  }

  Serial.println("LoRa success");
  delay(10);
}


void loop() {
  TakeMeasure();
  for (int j=0;j<=70;j++){
   LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
  delay(20);
}

void TakeMeasure(){
   // set the ADC resolution to 12 bits and read A0
  //analogReadResolution(12);
  //Serial.print(", 12-bit : ");
  average_value1=0;
  digitalWrite(4,HIGH);
  delay(200);
  for (int i=1;i<=5;i++){
  int sensor_value1 = analogRead(A0);
  average_value1 +=sensor_value1;
 // Serial.println(i);
  delay(50);
  }
  average_value1 =average_value1/5;
  
  int sensor_value2 = analogRead(A1);
  delay(5);
 // Serial.println(average_value1);
 // Serial.println(sensor_value2);
 // Serial.print("Sending packet: ");
  Serial.println("{\"ID\":\"02\",\"value1\":" + String(average_value1) + ",\"value2\":" + String(sensor_value2)+"}");
  delay(100);
  digitalWrite(4,LOW);
  delay(100);
  // send packet
  LoRa.beginPacket();
  LoRa.print("{\"ID\":\"01\",\"value1\":" + String(average_value1) + ",\"value2\":" + String(sensor_value2)+"}");
  delay(50);
  LoRa.endPacket();
  delay(50);
  
}

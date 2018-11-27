
/*
Note: this code is a demo for how to use a gprs shield to send data baterry and analog
an http request to a test website (using the Hologram APN).

To communicate with the Arduino via terminal, set
the outgoing baud rate to 19200, and set line endings to
carriage return.

Then, in order to initiate the demo http request, enter
'h' into the terminal at the top of the serial monitor.
*/

#include <Wire.h>

#include "SparkFun_BNO080_Arduino_Library.h"

#define SerialAT Serial2


//#define SerialAT Serial1
BNO080 myIMU;


int i=0;
float quatI =0.00;
float quatJ =0.00;
float quatK =0.00;
float quatReal =0.00;
float x=0.00;
float y=0.00;
float z=0.00;

void setup()
{
  int i=0;
  pinMode(14,OUTPUT);
  pinMode(13,OUTPUT);
  digitalWrite(14,LOW);
  digitalWrite(13,LOW);
  delay(2000);
  digitalWrite(13,HIGH);
  digitalWrite(14,HIGH);
  delay(15000);
  SerialAT.begin(9600); // the GPRS baud rate
  delay(500);
  //Serial.begin(9600); // the GPRS baud rate
  delay(500);
  
  Wire.begin();
  Wire.setClock(400000); //Increase I2C data rate to 400kHz
  myIMU.begin();
  myIMU.enableRotationVector(500); //Send data update every 50ms
  myIMU.enableLinearAccelerometer(500); //Send data update every 50ms
   delay(1000);
}

void loop()
{
  i++;
  get_quat();
  delay(700);
  if(i>10){
           i=0;
            digitalWrite(14,LOW);
            digitalWrite(13,LOW);
            delay(2000);
            digitalWrite(13,HIGH);
            digitalWrite(14,HIGH);
            delay(15000);
            SerialAT.begin(9600); // the GPRS baud rate
            delay(500);
            //Serial.begin(9600); // the GPRS baud rate
            delay(500);
            
            Wire.begin();
            Wire.setClock(400000); //Increase I2C data rate to 400kHz
            myIMU.begin();
            myIMU.enableRotationVector(500); //Send data update every 50ms
            myIMU.enableLinearAccelerometer(500); //Send data update every 50ms
             delay(1000);
        }
}

void get_quat(){
  if (myIMU.dataAvailable() == true)
   {
     quatI = myIMU.getQuatI();
     quatJ = myIMU.getQuatJ();
     quatK = myIMU.getQuatK();
     quatReal = myIMU.getQuatReal();
     x = myIMU.getLinAccelX();
     y = myIMU.getLinAccelY();
     z = myIMU.getLinAccelZ();
    /*  Serial.print(quatI, 2);
      Serial.print(F(","));
      Serial.print(quatJ, 2);
      Serial.print(F(","));
      Serial.print(quatK, 2);
      Serial.print(F(","));
      Serial.print(quatReal, 2);
      Serial.println("");
      
      Serial.write("submitHttp");
      */
      SubmitHttpRequest();
      //Serial.write("end");
      
       delay(100);
       
   }
   }

// SubmitHttpRequest()
//
// Note: the time of the delays are very important
void SubmitHttpRequest()
{
    
   digitalWrite(14,HIGH);
   digitalWrite(13,HIGH);
   delay(3000);
   SerialAT.println("AT");
   delay(1000);
   ShowSerialData();
  // Query signal strength of device
  SerialAT.println("AT+CSQ");
  delay(1000);

  ShowSerialData();

  SerialAT.println("AT+COPS?");
  delay(1000);

  ShowSerialData();

  // Check the status of Packet service attach. '0' implies device is not attached and '1' implies device is attached.
  SerialAT.println("AT+CGATT?");
  delay(1000);

  ShowSerialData();

  // Set the SAPBR, the connection type is using gprs
  SerialAT.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(500);

  ShowSerialData();

  // Set the APN
  SerialAT.println("AT+SAPBR=3,1,\"APN\",\"e-connect\"");
  delay(2000);

  ShowSerialData();

  // Set the SAPBR, for detail you can refer to the AT command manual
  SerialAT.println("AT+SAPBR=1,1");
  delay(1000);

  ShowSerialData();

  // Init the HTTP request
  SerialAT.println("AT+HTTPINIT");

  delay(2000);
  ShowSerialData();

  // Set HTTP params, the second param is the website to request
  //SerialAT.println("AT+HTTPPARA=\"URL\",\"api.thingspeak.com/update?api_key=G4SDI9TYWFKHS4N1&field1=" + String(quatI) + "&field2=" + String(quatJ) + "&field3=" + String(quatK)+ "&field4=" + String(quatReal)+ "&field5=" + String(x)+ "&field6=" + String(y)+ "&field7=" + String(z)+"\"");
  //delay(1000); //--->for device1
  //SerialAT.println("AT+HTTPPARA=\"URL\",\"api.thingspeak.com/update?api_key=LJV749JX5UDE1QQA&field1=" + String(quatI) + "&field2=" + String(quatJ) + "&field3=" + String(quatK)+ "&field4=" + String(quatReal)+ "&field5=" + String(x)+ "&field6=" + String(y)+ "&field7=" + String(z)+"\"");
  //---->for device 3
  SerialAT.println("AT+HTTPPARA=\"URL\",\"api.thingspeak.com/update?api_key=OKTNZXJ8G9D0SLQD&field1=" + String(quatI) + "&field2=" + String(quatJ) + "&field3=" + String(quatK)+ "&field4=" + String(quatReal)+ "&field5=" + String(x)+ "&field6=" + String(y)+ "&field7=" + String(z)+"\"");
  delay(1000);
  
  ShowSerialData();

  //Set the context ID
  SerialAT.println("AT+HTTPPARA=\"CID\",1");
  delay(1000);

  ShowSerialData();

  // Submit the request
  SerialAT.println("AT+HTTPACTION=0");
  // The delay is very important, the delay time is base on the
  // return time from the website, if the return data is very
  // large, the time required might be longer.
  delay(2000);

  ShowSerialData();

  // Read the data from the accessed website
  SerialAT.println("AT+HTTPREAD");
  delay(5000);

  ShowSerialData();

  // Close the HTTP connection and display the data
  SerialAT.println("AT+HTTPTERM");
  delay(1000);
   //digitalWrite(14,LOW);
    digitalWrite(13,LOW);
   // delay(500);
}


void ShowSerialData()
{
  while(SerialAT.available()!=0)
    SerialAT.read();
    //Serial.write(SerialAT.read());
}


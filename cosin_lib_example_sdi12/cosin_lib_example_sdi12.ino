
/*
Note: this code is a demo for how to use a gprs shield to send data baterry and analog
an http request to a test website (using the Hologram APN).

To communicate with the Arduino via terminal, set
the outgoing baud rate to 19200, and set line endings to
carriage return.

Then, in order to initiate the demo http request, enter
'h' into the terminal at the top of the serial monitor.
*/
#include "LowPower.h"
#include <SDISerial.h>



//in order to recieve data you must choose a pin that supports interupts
#define DATALINE_PIN 2
#define INVERTED 1



//SoftwareSerial mySerial(8,7);

#define mySerial Serial
int sensorValue = 0;
int leafValue =0;
float leafVol =0.0;
float vol=0;
float batVol=0.0;
float soil_ec;
float soil_moisture;
float soil_temp;

SDISerial sdi_serial_connection(DATALINE_PIN, INVERTED);

char* get_measurement(){
  char* service_request = sdi_serial_connection.sdi_query("?M!",1000);
  //you can use the time returned above to wait for the service_request_complete
  char* service_request_complete = sdi_serial_connection.wait_for_response(1000);
  //dont worry about waiting too long it will return once it gets a response
  return sdi_serial_connection.sdi_query("?D0!",1000);
}

void setup()
{
  sdi_serial_connection.begin(); // start our SDI connection 
  pinMode(9,OUTPUT);
  digitalWrite(9,HIGH);
  delay(10000);
  mySerial.begin(9600); // the GPRS baud rate
  delay(100);
 // Serial.begin(9600); // the GPRS baud rate
  delay(500);
    
}

int j=0;
void loop()
{
  // Input 'h' to run the test HTTP program

  
 // Serial.write("submitHttp");
  SubmitHttpRequest();
 // Serial.write("end");
  
  for (int i=0;i<418;i++)
  {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  
  }//delay(540000);
  
 // if (mySerial.available())
  //  Serial.write(mySerial.read());
}

// SubmitHttpRequest()
//
// Note: the time of the delays are very important
void SubmitHttpRequest()
{
    soil_ec=0.00;
    soil_moisture=0.00;
    soil_temp=0.00;
   digitalWrite(9,HIGH);
   GetSensorData();
   delay(18000);
   mySerial.println("AT");
   delay(2000);
//   ShowSerialData();
  // Query signal strength of device
  mySerial.println("AT+CSQ");
  delay(1000);

//  ShowSerialData();

  mySerial.println("AT+COPS?");
  delay(1000);

 // ShowSerialData();

  // Check the status of Packet service attach. '0' implies device is not attached and '1' implies device is attached.
  mySerial.println("AT+CGATT?");
  delay(5000);

 // ShowSerialData();

  // Set the SAPBR, the connection type is using gprs
  mySerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(1000);

 // ShowSerialData();

  // Set the APN
  mySerial.println("AT+SAPBR=3,1,\"APN\",\"e-connect\"");
  delay(6000);

 // ShowSerialData();

  // Set the SAPBR, for detail you can refer to the AT command manual
  mySerial.println("AT+SAPBR=1,1");
  delay(2000);

 // ShowSerialData();

  // Init the HTTP request
  mySerial.println("AT+HTTPINIT");

  delay(2000);
//  ShowSerialData();

  // Set HTTP params, the second param is the website to request
  mySerial.println("AT+HTTPPARA=\"URL\",\"cosin.herokuapp.com/chtien18/write/val1=" + String(batVol) + "&val2=" + String(leafVol)  + "&val3=" + String(soil_ec)+ "&val4=" + String(soil_moisture)+ "&val5=" + String(soil_temp)+"\"");
  delay(1000);

 // ShowSerialData();

  //Set the context ID
  mySerial.println("AT+HTTPPARA=\"CID\",1");
  delay(1000);

 // ShowSerialData();

  // Submit the request
  mySerial.println("AT+HTTPACTION=0");
  // The delay is very important, the delay time is base on the
  // return time from the website, if the return data is very
  // large, the time required might be longer.
  delay(10000);

  //ShowSerialData();

  // Read the data from the accessed website
  mySerial.println("AT+HTTPREAD");
  delay(21000);

  //ShowSerialData();

  // Close the HTTP connection and display the data
  mySerial.println("AT+HTTPTERM");
  delay(21000);
   digitalWrite(9,LOW);
}

// ShowSerialData()
// This is to show the data from gprs shield, to help
// see how the gprs shield submits an http request.

void GetSensorData()
  {
  //Get leaf sensor from analog A0 and battery from A1
  sensorValue=analogRead(A1);
  vol=sensorValue * (5.0 / 1023.0);
  batVol=vol + vol/2;
  leafValue=analogRead(A0);
  leafVol=leafValue * (5.0 / 1023.0);
  
  uint8_t wait_for_response_ms = 1000;
  char* response = get_measurement(); // get measurement data

  String data=String(response);
  
  int dem=0;
  Serial.println(data);
  for (int f=0;f<data.length();f++){
    if (String(data[f])=="+")
    {
      dem++;
      if(dem==1){
        String ec;
        for (int k=f+1; k<f+6;k++)
          {
            ec+=data[k];
          }
        for (int k=0; k<ec.length();k++)
          {
            if (String(ec[k])=="+") ec[k]=0;
          }  
        //  Serial.println("EC:" + ec); 
          soil_ec=ec.toFloat();
        };
        
      if(dem==2){
        String moisture;
        for (int k=f+1; k<f+6;k++)
          {
            moisture+=data[k];
          }
        for (int k=0; k<moisture.length();k++)
          {
            if (String(moisture[k])=="+") moisture[k]=0;
          }  
          //Serial.println("Moisture:" + moisture); 
          soil_moisture=moisture.toFloat();
        };
       if(dem==3){
        String temp;
        for (int k=f+1; k<f+5;k++)
          {
            temp+=data[k];
          }
        for (int k=0; k<temp.length();k++)
          {
            if (String(temp[k])=="+") temp[k]=0;
          }  
          //Serial.println("Temp:" + temp); 
          soil_temp=temp.toFloat();
        };
      }
    }
  }



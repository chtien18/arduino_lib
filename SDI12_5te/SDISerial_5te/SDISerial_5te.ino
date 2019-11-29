#include <SDISerial.h>

/*
I used a few devices with no problem using a MEGA 2560 as well as an UNO.
Sketch was built with Arduino 1.0.4, however I also tested it under 1.0.0
Place the SDISerial folder in    "<ARDUINO_ROOT>/libraries"

with the 5TE 
the WHITE wire is power. 
   - I hooked it up to the arduino 5v output pin, however you could also connect it to a pin and drive power only when you wanted it
the RED wire is the DATA_LINE.
   - you must hook it up to a pin that can process interrupts (see link below)
   
the remaining wire must be connected to ground
*/

//in order to recieve data you must choose a pin that supports interupts
#define DATALINE_PIN 2
#define INVERTED 1
//see:   http://arduino.cc/en/Reference/attachInterrupt
//for pins that support interupts (2 or 3 typically)


SDISerial sdi_serial_connection(DATALINE_PIN, INVERTED);

char* get_measurement(){
	char* service_request = sdi_serial_connection.sdi_query("?M!",1000);
	//you can use the time returned above to wait for the service_request_complete
	char* service_request_complete = sdi_serial_connection.wait_for_response(1000);
	//dont worry about waiting too long it will return once it gets a response
	return sdi_serial_connection.sdi_query("?D0!",1000);
}

void setup(){
  sdi_serial_connection.begin(); // start our SDI connection 
  Serial.begin(9600); // start our uart
  Serial.println("OK INITIALIZED"); // startup string echo'd to our uart
  delay(3000); // startup delay to allow sensor to powerup and output its DDI serial string
}

int j=0;
void loop(){
  uint8_t wait_for_response_ms = 1000;
  char* response = get_measurement(); // get measurement data
  //if you you didnt need a response you could simply do
  //         sdi_serial_connection.sdi_cmd("0A1") 
 //String  data=response[0];
 // for (int i=0;i<data.length();i++) Serial.println(i);
  Serial.print("RECV:");Serial.println(response!=NULL&&response[0] != '\0'?response:"No Response!"); //just a debug print statement to the serial port

  String data=String(response);
  //data+="+";
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
          Serial.println("EC:" + ec); 
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
          Serial.println("Moisture:" + moisture); 
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
          Serial.println("Temp:" + temp); 
        };
        
      
    }
  }
  
  Serial.println();
 // String ec=String(response[2]) +String(response[3])+String(response[4])+String(response[5]);
  //Serial.println("EC=" + ec);
  String soil_moisture=String(response[7]) +String(response[8])+String(response[9])+String(response[10]);
  Serial.println("Soil Moisture=" +soil_moisture);
  String soil_temp=String(response[12]) +String(response[13])+String(response[14])+String(response[15]);
  Serial.println("Soil Moisture=" +soil_temp);
  delay(5000);
 
}

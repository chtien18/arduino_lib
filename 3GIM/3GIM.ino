// 3GIM(V2) sample sketch for Mega/Leonardo.. -- httpGET

#include "a3gim.h"

#define baudrate 	9600UL

const int powerPin = 7;     // 3gim power pin(If not using power control, 0 is set.)
const char *server = "api.thingspeak.com";
const char *path = "/update?api_key=IG54WX1LS4MVNJUE";
const int port = a3gsDEFAULT_PORT;

char res[a3gsMAX_RESULT_LENGTH+1];
int len;


const int analog1 = A0;
const int analog2 = A1;

int analog1_Value=0;
int analog2_Value=0;

String data;
void setup()
{
  Serial.begin(baudrate);
  delay(100);  // Wait for Start Serial Monitor
  Serial.println("Ready.");

}

void loop()
{
 
  send_data();
  for(int i=0;i<60;i++){
  delay(60000);//60second
  }
}

String getdata()
{
  String data_str="/update?api_key=IG54WX1LS4MVNJUE";
  Serial.println("Get data...");
  analog1_Value = analogRead(analog1);
  Serial.print("Analog1 value:");
  Serial.println(analog1_Value);
  
  analog2_Value = analogRead(analog2);
  Serial.print("Analog2 value:");
  Serial.println(analog2_Value);

  data_str +="&field1=";
  
  data_str +=analog1_Value;
  data_str +="&field2=";
  data_str +=analog2_Value;
  
  Serial.println(data_str);
  return data_str;
  }
  
void send_data()
{
  data=getdata();
  char dataStr[100];
  data.toCharArray(dataStr,100);
  Serial.println(dataStr);
  
  Serial.print("Initializing.. ");
  if (a3gs.start(powerPin) == 0 && a3gs.begin(0, baudrate) == 0) {
    Serial.println("Succeeded.");
    Serial.print("httpGET() requesting.. ");
    len = sizeof(res);
    if (a3gs.httpGET(server, port, dataStr, res, len, true) == 0) {
      Serial.println("OK!");
      Serial.print("[");
      Serial.print(res);
      Serial.println("]");
    }
    else {
      Serial.print("Can't get HTTP response from ");
      Serial.println(server);
    }
  }
  else
    Serial.println("Failed.");

  Serial.println("Shutdown..");
  a3gs.end();
  a3gs.shutdown();
  }


// END

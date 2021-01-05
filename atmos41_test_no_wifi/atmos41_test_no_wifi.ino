#include <math.h>
//#include <SoftwareSerial.h> /* please do not use!!! it will be conflicted

/*---------------SDI12---------- */
#include <SDI12.h>
#define SERIAL_BAUD 115200 /*!< The baud rate for the output serial port */
#define DATA_PIN 11        /*!< The pin of the SDI-12 data bus. Should be (for Mega - only these pins) :0, 11, 12, 13, 14, 15, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64), A11 (65), A12 (66), A13 (67), A14 (68), A15 (69)*/

SDI12 mySDI12(DATA_PIN); /** Define the SDI-12 bus */

float sensorValues[15] = {0};  // Create an array to receive data

String myCommand = "0R7!";

/*-------------------------------*/

#define DEBUG 1
#define TIME30S 30000
#define TIME01M 60000


  
//SoftwareSerial esp8266(16,17);
#define   esp8266 Serial1 
// Kết nối Route wifi
String _ssid = "Quan No";  // Cần thay đổi        
String _pass = "12345678";          // Cần thay đổi  

String _ipserver = "IoTlab.net.vn";    //IoTlab.net.vn


String _StationCode="G28CikIp59";  //NgaNam01
String _StationSecret="N1a6kvsAq5";

String _ServerPort="3000";         // cổng kết nối
String S_ID[14]={"201","202","203","204","205","206","207","208","209","210"
                  ,"211","212","213","214"};   //S_ID (Sensor_ID: ma loai du lieu cam bien, xem file word kem theo
                   
String sendESP8266(String ATCommand, int timeout, boolean debug)
{
  String response = "";

  esp8266.print(ATCommand);

  long int time = millis();

  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      char c = esp8266.read();
      response += c;
    }
  }
  if (debug) {
    Serial.print(response);
    Serial.println("-----------------------------------------");
  }
  return response;
}


char * Str2Buff(String str){
  int len=str.length();
  char *buf;
  str.toCharArray(buf,len);
  return buf;
}

/*
 * int n: n là số luong cam bien nhiet do trong kho
 * int *dvalue; mang chua gia tri do cam bien. 
 */
String getURL(int n, float *dvalue){
  String url = "POST /api/data/insertmulti?station_code=";
  url +=_StationCode +"&station_secret="+_StationSecret+"&data_stationType=1";
  for (int i=0; i<n; ++i){
    url += "&Items[";
    url +=i;
    url +="][data_value]=";
    url +=dvalue[i];  //Vi du dvalue[i] là gia tri do tinh theo don vi do cua cam bien thu i
    url +="&Items[";
    url +=i;
    url +="][datatype_id]=";
    url +=S_ID[i];    //ma loai du lieu cam bien, xem file word kèm theo.  
  }
  url += " HTTP/1.1\r\nHost: ";
  url += _ipserver;
  url += "\r\n";
  url += " Connection: close";
  url += "\r\n\r\n";

  return url;
}

void connectWifi(){
  Serial.println("connecting wifi...\n");
  sendESP8266("AT+RST\r\n", 3000, DEBUG);
  sendESP8266("AT+CWMODE=1\r\n", 3000, DEBUG); //1 for station only, 3 for router and station mode
  sendESP8266("AT+CWJAP=\"" + _ssid + "\",\"" + _pass + "\"\r\n", 10000, DEBUG); //3000
  sendESP8266("AT+CIPMUX=0\r\n", 3000, DEBUG);  // Can increate delay time 3000 or more if need  
}

/*---------------get data from SDI12 sensor--------------*/
void getSDI12Data(){
      mySDI12.sendCommand(myCommand);
    delay(1000);                    // wait a while for a response
    Serial.println(F("Receiving data"));  // For debugging
    
    for (int j = 0; j < 15; j++)
    {
      float result = mySDI12.parseFloat();
      sensorValues[j] = result;
      Serial.print(F("Data #"));  // For debugging
      Serial.print(j);  // For debugging
      Serial.print(F(": "));  // For debugging
      Serial.println(result);  // For debugging
    }

    mySDI12.flush();
    delay(300);
    Serial.println();
  }
/*----------------------------------------------------*/
  int n=14;
  float tmp[14];
  
void setup() {
  Serial.begin(SERIAL_BAUD);
  esp8266.begin(115200);
  
  /*--------Open SDI12 bus---------*/
  Serial.println("Opening SDI-12 bus...");
  mySDI12.begin();
  delay(500); 
  /*------------------------------*/
   
  Serial.println("Staring...\n");
  delay(TIME30S);
  connectWifi();
 }


void loop() {

   //Get data 
  // Vi du gia lap nhan du lieu tu n=16 cam bien
  // Neu so cam bien it hon thi thay doi so luong n
/*---------call getSDI12Data ----------------*/  
  getSDI12Data();
/*-------------------------------------------*/
  
  for(int i=0;i<n;++i)
  {
    tmp[i]=sensorValues[i+1];
    Serial.println(tmp[i]);
  }
  // Tao URL de gui du lieu  
  
  delay(100);
  
  String url = getURL(n,tmp);
  Serial.println(url+"\n");
 /* 
  //Ket noi wifi gui du lieu - thay doi ket noi tuy theo phan cung 
  connectWifi();
  
  sendESP8266("AT+CIPSTART=\"TCP\",\"" + _ipserver + "\","+_ServerPort+"\r\n", 1000, DEBUG); 
  String cipsend = "AT+CIPSEND=";
  cipsend += url.length();

  cipsend += "\r\n";
 
  delay(3000);
  sendESP8266(cipsend, 1000, DEBUG); //1000
  delay(3000);
  sendESP8266(url,6000, DEBUG); //6000
  */
  
  //delay(TIME01M*4+TIME30S);  //delay 4.5 m
  delay(5000);
}

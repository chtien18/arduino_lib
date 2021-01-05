#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

char* ssid = "Quan No";
String pass = "12345678";

String _ipserver = "IoTlab.net.vn";    //IoTlab.net.vn


//String _StationCode="GclzSE0fra";  //Tram 02- đo ẩm độ đất, mực nước
//String _StationSecret="bEMv1R0o81";

String _StationCode="G28CikIp59";  //NgaNam01
String _StationSecret="N1a6kvsAq5";

String _ServerPort="3000";         // cổng kết nối
String S_ID[14]={"201","202","203","204","205","206","207","208","209","210"
                  ,"211","212","213","214"};   //S_ID (Sensor_ID: ma loai du lieu cam bien, xem file word kem theo
                  
//String _ServerPort="3000";         // cổng kết nối
//String S_ID[17]={"201","202","203","204","205","206","207","208","209","210"
//                 ,"211","212","213","214","070","071", "072"};   //S_ID (Sensor_ID: ma loai du lieu cam bien, xem file word kem theo
                   

void wifi_connect (){
  Serial.printf("Connection status: %d\n", WiFi.status());
    Serial.printf("Connecting to %s\n", ssid);
    WiFi.begin(ssid, pass);
    Serial.printf("Connection status: %d\n", WiFi.status());
    while (WiFi.status() != WL_CONNECTED)
    {
    delay(500);
    Serial.print(".");
    }
    Serial.printf("\nConnection status: %d\n", WiFi.status());
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
}
    
String getURL(int n, float *dvalue){
  //String url="&data_stationType=1";
  String url = "station_code=";
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


  return url;
}
  int n=2;
  float tmp[2];
  
void setup(void)
{
    Serial.begin(9600);
    wifi_connect();
}

void loop() {
  
     for(int i=0;i<n;++i)
  {
    tmp[i]=100;
    Serial.println(tmp[i]);
  }
  // Tao URL de gui du lieu  
  String url = getURL(n,tmp);
  
  Serial.println(url+"\n");
  
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    HTTPClient http;    //Declare object of class HTTPClient
 
   // http.begin("http://IoTlab.net.vn:3000/api/data/insertmulti?station_code=G28CikIp59&station_secret=N1a6kvsAq5&data_stationType=1&Items[0][data_value]=101.00&Items[0][datatype_id]=201");      //Specify request destination
    http.begin("http://IoTlab.net.vn:3000/api/data/insertmulti?station_code=GclzSE0fra&station_secret=N1a6kvsAq5&data_stationType=1&Items[0][data_value]=101.00&Items[0][datatype_id]=070");  
    //http.addHeader("Content-Type", "application/json");  //Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
 
    int httpCode = http.POST("");   //Send the request
    //int httpCode = http.sendRequest("POST","station_code=G28CikIp59&station_secret=N1a6kvsAq5&data_stationType=1&Items[0][data_value]=100.00&Items[0][datatype_id]=201&Items[1][data_value]=100.00&Items[1][datatype_id]=202");
    String payload = http.getString();                  //Get the response payload
 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
 
    http.end();  //Close connection
 
  } else {
 
    Serial.println("Error in WiFi connection");
 
  }
  delay(100000);
  }

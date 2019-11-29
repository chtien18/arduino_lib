// This example uses an Adafruit Huzzah ESP8266

#include <ESP8266WiFi.h>


#include <Wire.h>
#include "Adafruit_SHT31.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();

const char *ssid = "Cosin";
const char *pass = "12345678";


  float t = 0;
  float h = 0;

const char* host = "172.16.1.1";
   WiFiClient client;
   
void setup() {
   Serial.begin(9600);
 Serial.println("SHT31 test");
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

  /* We're ready to go! */
  Serial.println("");
  
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
}


void loop() {
   WiFi.begin(ssid, pass);

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections

  const int httpPort = 1880;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
sht31_mesurement();

 // We now create a URI for the request
  String url = "/write/device2/data/{\"temp\":"+(String)t + ",\"humi\":"+(String)h + "}";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
  client.stop();
  
  delay(18000);

}


void sht31_mesurement(){
   t = sht31.readTemperature();
   h = sht31.readHumidity();

  if (! isnan(t)) {  // check if 'is not a number'
    Serial.print("Temp *C = "); Serial.println(t);
  } else { 
    Serial.println("Failed to read temperature");
  }
  
  if (! isnan(h)) {  // check if 'is not a number'
    Serial.print("Hum. % = "); Serial.println(h);
  } else { 
    Serial.println("Failed to read humidity");
  }
  Serial.println();
  delay(1000);
         
    
  }


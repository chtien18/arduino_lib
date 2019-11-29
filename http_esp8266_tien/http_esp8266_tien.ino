/*
 *  Simple HTTP get webclient test
 */
 
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN            13         // Pin which is connected to the DHT sensor.
//#define DHTTYPE           DHT11     // DHT 11 
#define DHTTYPE             DHT22

float dht11_humidity =0;
float dht11_temperature =0;
DHT_Unified dht(DHTPIN, DHTTYPE);

const char* ssid     = "Cosin";
const char* password = "12345678";
 
const char* host = "172.16.1.1";
   WiFiClient client;
void setup() {
  Serial.begin(115200);
  delay(100);
  dht.begin();
  // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
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
 
int value = 0;
 
void loop() {

 
  WiFi.begin(ssid, password);

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections

  const int httpPort = 1880;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  //read sensor
  dht11_mesurement();
  
  // We now create a URI for the request
  String url = "/write/device1/data/{\"temp\":"+(String)dht11_temperature + ",\"humi\":"+(String)dht11_humidity + "}";
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

void dht11_mesurement(){
         delay(100);
      // Get temperature event and print its value.
      sensors_event_t event;  
      dht.temperature().getEvent(&event);
      if (isnan(event.temperature)) {
        Serial.println("Error reading temperature!");
      }
      else {
        Serial.print("Temperature: ");
        dht11_temperature=event.temperature;
        Serial.print(event.temperature);
        Serial.println(" *C");
      }
      // Get humidity event and print its value.
      dht.humidity().getEvent(&event);
      if (isnan(event.relative_humidity)) {
        Serial.println("Error reading humidity!");
      }
      else {
        Serial.print("Humidity: ");
        Serial.print(event.relative_humidity);
        dht11_humidity=event.relative_humidity;
        Serial.println("%");
      }
    
  }

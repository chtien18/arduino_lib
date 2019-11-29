// This example uses an Adafruit Huzzah ESP8266
// to connect to shiftr.io.
//
// You can check on your device after a successful
// connection here: https://shiftr.io/try.
//
// by Joël Gähwiler
// https://github.com/256dpi/arduino-mqtt

#include <ESP8266WiFi.h>
#include <MQTTClient.h>

#include <Wire.h>
#include "Adafruit_SHT31.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();

const char *ssid = "agrinode";
const char *pass = "12345678";


  float t = 0;
  float h = 0;

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;



uint32_t delayMS;


void connect(); // <- predefine connect() for setup()

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
  client.begin("172.16.1.1", net);

  connect();
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino02", "chtien19", "iotsample-arduino")) {  //client, user, pass
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("node01");
  // client.unsubscribe("/example");
}

void loop() {
  client.loop();
  delay(100); // <- fixes some issues with WiFi stability

  if(!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
 // if(millis() - lastMillis > 5000) {
 //   lastMillis = millis();
 //   client.publish("/hello", "world");
 // }
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();

  if(payload[0]=='1'){
    
    sht31_mesurement();
 
    
    String json=buildJson();
    char jsonStr[200];
    json.toCharArray(jsonStr,200);
    Serial.print(jsonStr);
    client.publish("node02_data",jsonStr);
    }
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

String buildJson() {
  String data = "{";

  data+="\"ID\": \"Node02\",";

  data+="\"Humidity\": ";
  data+=(float)h;
  data+= ",";
  //data+="\n";
  data+="\"Temperature\": ";
  data+=(float)t;
 //data+= ",";
  //data+="\n";
 
  data+="}";
  data+="\n";
  //data+="}";
  return data;
}

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
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN            12         // Pin which is connected to the DHT sensor.
#define DHTTYPE           DHT11     // DHT 11 

const char *ssid = "agrinode";
const char *pass = "12345678";

float dht11_humidity =0;
float dht11_temperature =0;

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;


void connect(); // <- predefine connect() for setup()

void setup() {
   Serial.begin(9600);
  dht.begin();
  Serial.println("DHTxx Unified Sensor Example");
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Temperature");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
  Serial.println("------------------------------------");
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Humidity");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  
  Serial.println("------------------------------------");
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
  
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
    
    dht11_mesurement();
 
    
    String json=buildJson();
    char jsonStr[200];
    json.toCharArray(jsonStr,200);
    Serial.print(jsonStr);
    client.publish("node02_data",jsonStr);
    }
}

void dht11_mesurement(){
         delay(delayMS);
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

String buildJson() {
  String data = "{";

  data+="\"ID\": \"Node02\",";

  data+="\"Humidity\": ";
  data+=(float)dht11_humidity;
  data+= ",";
  //data+="\n";
  data+="\"Temperature\": ";
  data+=(float)dht11_temperature;
 //data+= ",";
  //data+="\n";
 
  data+="}";
  data+="\n";
  //data+="}";
  return data;
}

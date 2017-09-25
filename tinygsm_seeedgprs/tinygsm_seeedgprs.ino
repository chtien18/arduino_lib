/**************************************************************
 *
 * For this example, you need to install PubSubClient library:
 *   https://github.com/knolleary/pubsubclient/releases/latest
 *   or from http://librarymanager/all#PubSubClient
 *
 * TinyGSM Getting Started guide:
 *   http://tiny.cc/tiny-gsm-readme
 *
 **************************************************************
 * Use Mosquitto client tools to work with MQTT
 *   Ubuntu/Linux: sudo apt-get install mosquitto-clients
 *   Windows:      https://mosquitto.org/download/
 *
 * Subscribe for messages:
 *   mosquitto_sub -h test.mosquitto.org -t GsmClientTest/init -t GsmClientTest/ledStatus -q 1
 * Toggle led:
 *   mosquitto_pub -h test.mosquitto.org -t GsmClientTest/led -q 1 -m "toggle"
 *
 * You can use Node-RED for wiring together MQTT-enabled devices
 *   https://nodered.org/
 * Also, take a look at these additional Node-RED modules:
 *   node-red-contrib-blynk-websockets
 *   node-red-dashboard
 *
 **************************************************************/

// Select your modem:
#define TINY_GSM_MODEM_SIM800
// #define TINY_GSM_MODEM_SIM900
// #define TINY_GSM_MODEM_A6
// #define TINY_GSM_MODEM_A7
// #define TINY_GSM_MODEM_M590
// #define TINY_GSM_MODEM_ESP8266
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include "gprs.h"

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "e-connect";
const char user[] = "";
const char pass[] = "";

// Use Hardware Serial on Mega, Leonardo, Micro
//#define SerialAT Serial1

// or Software Serial on Uno, Nano
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(8, 7); // RX, TX

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient mqtt(client);
GPRS gprs(9600);

const char* broker = "mqtt.thingspeak.com";


// DHT Sensor connected to digital pin 2
#define DHTPIN 10
// Type of DHT sensor
#define DHTTYPE DHT22 
// Analog light sensor connected to analog pin A0
#define LIGHTPIN A0 
// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

Adafruit_BMP085 bmp;
#define LED_PIN 13
int ledStatus = LOW;

long lastReconnectAttempt = 0;
// track the last connection time
unsigned long lastConnectionTime = 0; 
// post data every 600 seconds (10 minute)
const unsigned long postingInterval = 600L * 1000L; 

void setup() {
  pinMode(LED_PIN, OUTPUT);

  // Set console baud rate
  Serial.begin(9600);
  delay(10);


  // Set GSM module baud rate
  SerialAT.begin(9600);
  delay(3000);
dht.begin();
delay(100);
bmp.begin();
delay(100);
 delay(100);
  gprs.preInit();
  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  Serial.println("Initializing modem...");
  modem.restart();

  // Unlock your SIM card with a PIN
  //modem.simUnlock("1234");

  Serial.print("Waiting for network...");
  if (!modem.waitForNetwork()) {
    Serial.println(" fail");
    while (true);
  }
  Serial.println(" OK");

  Serial.print("Connecting to ");
  Serial.print(apn);
  if (!modem.gprsConnect(apn, user, pass)) {
    Serial.println(" fail");
    while (true);
  }
  Serial.println(" OK");

  // MQTT Broker setup
  mqtt.setServer(broker, 1883);
//  mqtt.setCallback(mqttCallback);
}

boolean mqttConnect() {
  Serial.print("Connecting to ");
  Serial.print(broker);
  if (!mqtt.connect("agrinode01")) {
    Serial.println(" fail");
    return false;
  }
  Serial.println(" OK");
    mqttpublish();
  //mqtt.publish(topicInit, "GsmClientTest started");
  //mqtt.subscribe(topicLed);
 //  String data = String("field1=" + String(t, DEC) + "&field2=" + String(h, DEC) + "&field3=" + String(t_baro, DEC) + "&field4=" + String(pressure, DEC) + "&field5=" + String(altitude, DEC) + "&field6=" + String(sea_level, DEC) + "&field7=" + String(23, DEC) );
 
  return mqtt.connected();
}

void loop() {

  if (mqtt.connected()) {
    mqtt.loop();
    // If interval time has passed since the last connection, Publish data to ThingSpeak
    if (millis() - lastConnectionTime > postingInterval) 
    {
    mqttpublish();
    }
  } else {
    // Reconnect every 10 seconds
    unsigned long t = millis();
    if (t - lastReconnectAttempt > 10000L) {
      lastReconnectAttempt = t;
      if (mqttConnect()) {
        lastReconnectAttempt = 0;
      }
    }
  }

}

void mqttpublish() {
 // Read temperature from DHT sensor
  float t = dht.readTemperature();
  // Read humidity from DHT sensor
  float h = dht.readHumidity();
// read temp from barometric sensor
  float t_baro= bmp.readTemperature();
  float pressure = bmp.readPressure();
  float altitude = bmp.readAltitude();
  float sea_level = bmp.readSealevelPressure();
  
  // Read from light sensor
  int lightLevel = analogRead(LIGHTPIN);
  //Resisten of light
  float Rsensor = (float)(1023-lightLevel)*10/lightLevel; 
  // Create data string to send to ThingSpeak
 // String data = String("field1=" + String(t, DEC) + "&field2=" + String(h, DEC) + "&field3=" + String(t_baro, DEC) + "&field4=" + String(pressure, DEC) + "&field5=" + String(altitude, DEC) + "&field6=" + String(sea_level, DEC) + "&field7=" + String(Rsensor, DEC) );
 String data = String("field1=" + String(t) + "&field2=" + String(h) + "&field3=" + String(t_baro) + "&field4=" + String(pressure) + "&field5=" + String(altitude)); 
  // Get the data string length
  int length = data.length();
  char msgBuffer[length];
  // Convert data string to character buffer
  data.toCharArray(msgBuffer,length);
  Serial.println(msgBuffer);
  // Publish data to ThingSpeak. Replace <YOUR-CHANNEL-ID> with your channel ID and <YOUR-CHANNEL-WRITEAPIKEY> with your write API key
  mqtt.publish("channels/160962/publish/PZ8PVWP7JMEA85XO",msgBuffer);
  // note the last connection time
  lastConnectionTime = millis();
}


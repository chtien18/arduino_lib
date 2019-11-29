/**************************************************************
 *
 * For this example, you need to install PubSubClient library:
 *   https://github.com/knolleary/pubsubclient
 *   or from http://librarymanager/all#PubSubClient
 *
 * TinyGSM Getting Started guide:
 *   http://tiny.cc/tiny-gsm-readme
 *
 * For more MQTT examples, see PubSubClient library
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
 *   node-red-contrib-blynk-ws
 *   node-red-dashboard
 *
 **************************************************************/

// Select your modem:
#define TINY_GSM_MODEM_SIM800
// #define TINY_GSM_MODEM_SIM808
// #define TINY_GSM_MODEM_SIM900
// #define TINY_GSM_MODEM_A6
// #define TINY_GSM_MODEM_A7
// #define TINY_GSM_MODEM_M590
// #define TINY_GSM_MODEM_ESP8266
// #define TINY_GSM_MODEM_XBEE

#include <TinyGsmClient.h>
#include <PubSubClient.h>

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial
#define powerpin 9
// Use Hardware Serial on Mega, Leonardo, Micro
//#define SerialAT Serial1

// or Software Serial on Uno, Nano
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(8, 7); // RX, TX


// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "e-connect";
const char user[] = "";
const char pass[] = "";

//E-tape
const int etapePin = A0;

int etapeValue = 0;

// MQTT details
const char* broker = "35.226.242.249";

const char* topicLed = "etape1";

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient mqtt(client);

#define LED_PIN 13
int ledStatus = LOW;

long lastReconnectAttempt = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
 pinMode(powerpin,OUTPUT);
  // Set console baud rate
  SerialMon.begin(9600);
  delay(10);

  // Set GSM module baud rate
  SerialAT.begin(9600);
  delay(3000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println("Initializing modem...");
  modem.restart();

  String modemInfo = modem.getModemInfo();
  SerialMon.print("Modem: ");
  SerialMon.println(modemInfo);
  toggle();
  // Unlock your SIM card with a PIN
  //modem.simUnlock("1234");
}
  

void loop() {

  
  SerialMon.print("Waiting for network...");
  if (!modem.waitForNetwork()) {
    SerialMon.println(" fail");
            digitalWrite(powerpin,LOW);
            delay(800);
            digitalWrite(powerpin,HIGH);
            delay(200);
            digitalWrite(powerpin,LOW);
            delay(2000);
            digitalWrite(powerpin,HIGH);
            delay(3000);  
    delay(10000);
    return;
  }
  SerialMon.println(" OK");

  SerialMon.print("Connecting to ");
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, user, pass)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
      }
  SerialMon.println(" OK");

  // MQTT Broker setup
  mqtt.setServer(broker, 1883);

     //read etape

     mqttPublish();
  
  mqtt.loop();
  delay(5000);
  toggle(); //toggle gSM module power
  delay(30000);
}

void toggle(){
  digitalWrite(powerpin,LOW);
     delay(800);
            digitalWrite(powerpin,HIGH);
            delay(200);
            digitalWrite(powerpin,LOW);
            delay(2000);
            digitalWrite(powerpin,HIGH);
            delay(3000);  }
            
boolean mqttPublish() {
  SerialMon.print("Connecting to ");
  SerialMon.print(broker);
  if (!mqtt.connect("etape1","etape1","12345678")) {
    SerialMon.println(" fail");
    return false;
  }
  SerialMon.println(" OK");
  etapeValue=analogRead(etapePin);
  String json =buildjson();
  char jsonstr[50];
  json.toCharArray(jsonstr,50);
  Serial.println(jsonstr);
  mqtt.publish(topicLed, jsonstr);
  Serial.println("Success");
  return mqtt.connected();
}

String buildjson (){
  String data="{";
  data += "\"id\":\"etape1\",\"value\":";
  data += etapeValue;
  data += "}";
  return data;
  }



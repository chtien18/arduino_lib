/**************************************************************
 *
 * This sketch connects to a website and downloads a page.
 * It can be used to perform HTTP/RESTful API calls.
 *
 * TinyGSM Getting Started guide:
 *   http://tiny.cc/tiny-gsm-readme
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

#include <TinyGsmClient.h>
#include <LowPower.h>
// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "e-connect";
const char user[] = "";
const char pass[] = "";

// Use Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial1

// or Software Serial on Uno, Nano
//#include <SoftwareSerial.h>
//SoftwareSerial SerialAT(2, 3); // RX, TX

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);

const char server[] = "cosin.herokuapp.com";
const char resource[] = "/chtien18/write/val1=34&val2=78";

const int port = 80;

void setup() {
  pinMode(7,OUTPUT);
  // Set console baud rate
  Serial.begin(9600);
  delay(10);

  // Set GSM module baud rate
  SerialAT.begin(9600);
  delay(3000);

 
  // Unlock your SIM card with a PIN
  //modem.simUnlock("1234");
}

void loop() {
  digitalWrite(7,LOW);
  delay(1000);
  digitalWrite(7,HIGH);
  delay(17000);
   // Restart takes quite some time
  // To skip it, call init() instead of restart()
  Serial.println(F("Initializing modem..."));
  modem.restart();
  delay(100);
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem: ");
  Serial.println(modemInfo);

  
  Serial.print(F("Waiting for network..."));
  if (!modem.waitForNetwork()) {
    Serial.println(" fail");
    delay(1000);
    return;
  }
  Serial.println(" OK");

  Serial.print(F("Connecting to "));
  Serial.print(apn);
  if (!modem.gprsConnect(apn, user, pass)) {
    Serial.println(" fail");
    delay(1000);
    return;
  }
  Serial.println(" OK");

  Serial.print(F("Connecting to "));
  Serial.print(server);
  if (!client.connect(server, port)) {
    Serial.println(" fail");
    
    delay(1000);
    return;
  }
  Serial.println(" OK");

  // Make a HTTP GET request:
  client.print(String("GET ") + resource + " HTTP/1.0\r\n");
  client.print(String("Host: ") + server + "\r\n");
  client.print("Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.connected() && millis() - timeout < 20000L) {
    // Print available data
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
      timeout = millis();
    }
  }
  Serial.println();

  client.stop();
  Serial.println("Server disconnected");

  modem.gprsDisconnect();
  Serial.println("GPRS disconnected");
  
  digitalWrite(7,LOW);

  // Do nothing forevermore
  
    delay(60000);
  
}


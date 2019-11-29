/**************************************************************
 *
 * This sketch connects to a website and downloads a page.
 * It can be used to perform HTTP/RESTful API calls.
 *
 * For this example, you need to install ArduinoHttpClient library:
 *   https://github.com/arduino-libraries/ArduinoHttpClient
 *   or from http://librarymanager/all#ArduinoHttpClient
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

// Increase RX buffer
#define TINY_GSM_RX_BUFFER 600

// Use Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial1

// or Software Serial on Uno, Nano
//#include <SoftwareSerial.h>
//SoftwareSerial SerialAT(2, 3); // RX, TX

//#define DUMP_AT_COMMANDS
//#define TINY_GSM_DEBUG Serial


// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "v-connect";
const char user[] = "";
const char pass[] = "";

// Name of the server we want to connect to
const char server[] = "cosin.herokuapp.com";
const int  port     = 80;
// Path to download (this is the bit after the hostname in the URL)
const char resource[] = "/chtien18/write/val1=40&val2=45";

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
HttpClient http(client, server, port);

void setup() {
  pinMode(7,OUTPUT);
  pinMode(13,OUTPUT);
  delay(10);
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
  digitalWrite(13,HIGH);
  Serial.begin(9600);
  delay(10);

  // Set GSM module baud rate
  SerialAT.begin(9600);
  delay(3000);
  digitalWrite(7,LOW);
  delay(1000);
  digitalWrite(7,HIGH); //power on sim800h
  delay(3000);
  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  Serial.println("Initializing modem...");
  modem.restart();
  
  delay(1000);
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem: ");
  delay(100);
  Serial.println(modemInfo);
  delay(100);
  Serial.print(F("Waiting for network..."));
  if (!modem.waitForNetwork()) {
    Serial.println(" fail");
    delay(1000);
    return;
  }
  delay(100);
  Serial.println(" OK");

  Serial.print(F("Connecting to "));
  Serial.print(apn);
  if (!modem.gprsConnect(apn, user, pass)) {
    Serial.println(" fail");
    delay(1000);
    return;
  }
  delay(100);
  Serial.println(" OK");

  Serial.print(F("Performing HTTP GET request... "));
  int err = http.get(resource);
  if (err != 0) {
    Serial.println("failed to connect");
    delay(1000);
    return;
  }

  // Shutdown
  int status = http.responseStatusCode();
  Serial.println(status);
  if (!status) {
    delay(1000);
    return;
  }

  http.stop();

  modem.gprsDisconnect();
  delay(10);
  Serial.println("GPRS disconnected");

  delay(5000);
  modem.restart();
  delay(100);
  digitalWrite(7,LOW);//power off sim800h
  
  // Do nothing forevermore
  // ATmega32U4
  digitalWrite(13,LOW);
    delay(600000);
}


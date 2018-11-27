/*
  Using the BNO080 IMU
  By: Nathan Seidle
  SparkFun Electronics
  Date: December 21st, 2017
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14586

  This example shows how to output the i/j/k/real parts of the rotation vector.
  https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation

  It takes about 1ms at 400kHz I2C to read a record from the sensor, but we are polling the sensor continually
  between updates from the sensor. Use the interrupt pin on the BNO080 breakout to avoid polling.

  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the sensor onto the shield
  Serial.print it out at 9600 baud to serial monitor.
*/
#define TINY_GSM_MODEM_SIM800

#include <TinyGsmClient.h>

#include <Wire.h>

#include "SparkFun_BNO080_Arduino_Library.h"
#define CPU_REBOOT (_reboot_Teensyduino_());
BNO080 myIMU;


//SoftwareSerial serialSIM800(SIM800_TX_PIN,SIM800_RX_PIN);
#define serialSIM800 Serial2
#define TIMEOUT 30000

int8_t answer;

int onModulePin = 14;

char aux_str[100];

int x = 0;

int attempts;

char apn[] = "e-connect";

char url[ ]="api.thingspeak.com/update?api_key=PHRFH37I50UK9MGF&field1=";


unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 15000L; // delay between updates, in milliseconds
int i=0;

void setup()
{
  i=0;
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
  Serial.begin(9600);
  Serial.println();
  Serial.println("BNO080 Read Example");

  pinMode(onModulePin, OUTPUT);
  
  //Begin serial comunication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  while(!Serial);
   
  //Being serial communication with Arduino and SIM800
  serialSIM800.begin(9600);
  delay(1000);
   
  Serial.println("Starting...");
  
  power_on();

  delay(3000);
  
  //Serial.println("AT+HTTPPARA=\"URL\",\"www.castillolk.com.ve/WhiteList.txt\"\r\n");
  
  while( (sendATcommand("AT+CREG?\r\n", "+CREG: 0,1\r\n", 500) || 
            sendATcommand("AT+CREG?\r\n", "+CREG: 0,5\r\n", 500)) == 0 );
  connectToNetwork();
  initHTTPSession();
  HTTPRequest();

  Wire.begin();
  Wire.setClock(400000); //Increase I2C data rate to 400kHz

  myIMU.begin();

  myIMU.enableRotationVector(50); //Send data update every 50ms

  Serial.println(F("Rotation vector enabled"));
  Serial.println(F("Output in form i, j, k, real, accuracy"));
}

void loop()
{
 get_quat();
 
 delay(500);
 if (i>10){
  i=0;
  CPU_REBOOT;
 }
 i++;
}
void get_quat(){
   //Look for reports from the IMU
  if (myIMU.dataAvailable() == true)
  {
    float quatI = myIMU.getQuatI();
    float quatJ = myIMU.getQuatJ();
    float quatK = myIMU.getQuatK();
    float quatReal = myIMU.getQuatReal();
    float quatRadianAccuracy = myIMU.getQuatRadianAccuracy();

    Serial.print(quatI, 2);
    Serial.print(F(","));
    Serial.print(quatJ, 2);
    Serial.print(F(","));
    Serial.print(quatK, 2);
    Serial.print(F(","));
    Serial.print(quatReal, 2);
    Serial.print(F(","));
    Serial.print(quatRadianAccuracy, 2);
    Serial.print(F(","));

    Serial.println();
  }
  }

 /////////////////////////////////////////////////////////
int8_t sendATcommand(const char* ATcommand, const char* expected_answer1, unsigned int timeout) {

  uint8_t x = 0,  answer = 0;
  char response[100];
  unsigned long previous;

  memset(response, '\0', 100);    // Initialize the string

  delay(100);

  while (serialSIM800.available())
  { //Cleans the input buffer
    serialSIM800.read();
  }



  Serial.println(ATcommand);    // Prints the AT command
  serialSIM800.write(ATcommand); // Sends the AT command


  x = 0;
  previous = millis();

  // this loop waits for the answer
  do
  {
    ////if (Serial.available() != 0) {
    if (serialSIM800.available() != 0)
 {
      ////response[x] = Serial.read();
      response[x] = serialSIM800.read();
      x++;
      // check if the desired answer is in the response of the module
      if (strstr(response, expected_answer1) != NULL)
      {
        answer = 1;
      }
    }
    // Waits for the asnwer with time out
  }
  while ((answer == 0) && ((millis() - previous) < timeout));

  return answer;
}

/////////////////////////////////////////////////////////
void power_on()
{

  uint8_t answer = 0;

  Serial.println("On Power_on...");

  // checks if the module is started
  answer = sendATcommand("AT\r\n", "OK\r\n", TIMEOUT);
  if (answer == 0)
  {
    // power on pulse
    digitalWrite(onModulePin, LOW);
    delay(3000);
    digitalWrite(onModulePin, HIGH);

    // waits for an answer from the module
    while (answer == 0)
  {
      // Send AT every two seconds and wait for the answer
      answer = sendATcommand("AT\r\n", "OK\r\n", TIMEOUT);
      Serial.println("Trying connection with module...");
    }
  }
}

/////////////////////////////////////////////////////////
void restartPhoneActivity()
{
  do
  {
    sendATcommand("AT+CFUN=0\r\n", "OK\r\n", TIMEOUT);
    delay(2000);
    answer = sendATcommand("AT+CFUN=1\r\n", "Call Ready\r\n", TIMEOUT);
  }while(answer == 0);
}
/////////////////////////////////////////////////////////
void connectToNetwork()
{
  sendATcommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n", "OK\r\n", TIMEOUT);//sets Contype
  snprintf(aux_str, sizeof(aux_str), "AT+SAPBR=3,1,\"APN\",\"%s\"\r\n", apn);//sets APN
  sendATcommand(aux_str, "OK\r\n", TIMEOUT);
  attempts = 0;//tries 3 times or gets on the loop until sendATcommand != 0
  while (sendATcommand("AT+SAPBR=1,1\r\n", "OK\r\n", TIMEOUT) == 0)
    {
    delay(5000);
    attempts = attempts + 1;
    if(attempts > 2)
    {
      restartPhoneActivity();
      attempts = 0;
    }
    } 
}
/////////////////////////////////////////////////////////
void initHTTPSession()
{
  while (sendATcommand("AT+HTTPINIT\r\n", "OK\r\n", TIMEOUT) == 0)
    {
    restartPhoneActivity();
    connectToNetwork();
   }
}
/////////////////////////////////////////////////////////
void HTTPRequest()
{

  ////snprintf(aux_str, sizeof(aux_str), "AT+HTTPPARA=\"URL\",\"%s\"\"%f\"\r\n", url, light);
  //float light = readLDR();
  //char sensorValue[6];
  //dtostrf(light,5,2,sensorValue);
  snprintf(aux_str, sizeof(aux_str), "AT+HTTPPARA=\"URL\",\"%s\%s\"\r\n", url, "13");
  sendATcommand(aux_str, "OK\r\n", TIMEOUT);
  delay(3000);
  attempts = 0;//tries 3 times or gets on the loop until sendATcommand != 0
  while (sendATcommand("AT+HTTPACTION=0\r\n", "+HTTPACTION: 0,200,", TIMEOUT) == 0)
    {
    delay(5000);
    attempts = attempts + 1;
    if(attempts > 2)
    {
      sendATcommand("AT+SAPBR=0,1\r\n", "OK\r\n", 2 * TIMEOUT);
      sendATcommand("AT+HTTPTERM\r\n", "OK\r\n", TIMEOUT);
      restartPhoneActivity();
      connectToNetwork();
      initHTTPSession();
      ////snprintf(aux_str, sizeof(aux_str), "AT+HTTPPARA=\"URL\",\"%s\"\r\n", url);
     // light = readLDR();
     // char sensorValue[6];
     //dtostrf(light,5,2,sensorValue);
      snprintf(aux_str, sizeof(aux_str), "AT+HTTPPARA=\"URL\",\"%s\%s\"\r\n", url, "13");
      sendATcommand(aux_str, "OK\r\n", TIMEOUT);
      attempts = 0;
    }
    }
  Serial.println("Successfully uploaded");
  // note the time that the connection was made
    lastConnectionTime = millis();
  
}
/////////////////////////////////////////////////////////


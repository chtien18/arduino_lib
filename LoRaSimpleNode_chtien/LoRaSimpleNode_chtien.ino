
/*
  LoRa Simple Gateway/Node Exemple

  This code uses InvertIQ function to create a simple Gateway/Node logic.

  Gateway - Sends messages with enableInvertIQ()
          - Receives messages with disableInvertIQ()

  Node    - Sends messages with disableInvertIQ()
          - Receives messages with enableInvertIQ()

  With this arrangement a Gateway never receive messages from another Gateway
  and a Node never receive message from another Node.
  Only Gateway to Node and vice versa.

  This code receives messages and sends a message every second.

  InvertIQ function basically invert the LoRa I and Q signals.

  See the Semtech datasheet, http://www.semtech.com/images/datasheet/sx1276.pdf
  for more on InvertIQ register 0x33.

  created 05 August 2018
  by Luiz H. Cassettari
*/

#include <SPI.h>              // include libraries
#include <LoRa.h>

const long frequency = 433E6;  // LoRa Frequency
int average_value1 = 0;

const int csPin = 10;          // LoRa radio chip select
const int resetPin = 9;        // LoRa radio reset
const int irqPin = 1;          // change for your board; must be a hardware interrupt pin// pin 1 for arduino Leonardo, pin2 for others

const int ec5_power_pin =4;   //pin6 for leonardo, pin 4 for arduino mini

void setup() {
  pinMode(A0,INPUT);//EC5
  pinMode(A1,INPUT);//batt
  pinMode(ec5_power_pin,OUTPUT);
  digitalWrite(ec5_power_pin,LOW);
  Serial.begin(9600);                   // initialize serial
  //while (!Serial);

  LoRa.setPins(csPin, resetPin, irqPin);

  if (!LoRa.begin(frequency)) {
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
   
  }

  Serial.println("LoRa init succeeded.");
  Serial.println();
  Serial.println("LoRa Simple Node");
  Serial.println("Only receive messages from gateways");
  Serial.println("Tx: invertIQ disable");
  Serial.println("Rx: invertIQ enable");
  Serial.println();

  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);

  Take_measure();
  LoRa_rxMode();
}

void loop() {
  delay(5);
  
  //if (runEvery(5000)) { // repeat every 1000 millis
  //Take_measure();
// }
}

void Take_measure(){
   average_value1=0;
    digitalWrite(ec5_power_pin,HIGH);
    delay(200);
    for (int i=1;i<=5;i++){
    int sensor_value1 = analogRead(A0);
    average_value1 +=sensor_value1;
    delay(50);
    }
    average_value1 =average_value1/5;
  
    int sensor_value2 = analogRead(A1);
    delay(5);
    delay(100);
    digitalWrite(ec5_power_pin,LOW);
    delay(100);
  
    String message = "";
    message += "{\"ID\":\"02\",\"value1\":" + String(average_value1) + ",\"value2\":" + String(sensor_value2)+"}";

    LoRa_sendMessage(message); // send a message
    
    Serial.println("Send Message!");
    Serial.println(message);
  }
void LoRa_rxMode(){
  LoRa.enableInvertIQ();                // active invert I and Q signals
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.disableInvertIQ();               // normal mode
}

void LoRa_sendMessage(String message) {
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket(true);                 // finish packet and send it
}

void onReceive(int packetSize) {
  String message = "";
  while (LoRa.available()) {
    message += (char)LoRa.read();
  }

  Serial.print("Node Receive: ");
  Serial.println(message);
  
  delay(600);
  Take_measure();
}

void onTxDone() {
  Serial.println("TxDone");
  LoRa_rxMode();
}

boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}

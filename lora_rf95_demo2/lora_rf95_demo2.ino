// LoRa 9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example LoRa9x_RX

#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 434.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
//#define SerialUSB SerialUSBUSB
void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  //digitalWrite(RFM95_RST, HIGH);

  //while (!SerialUSB);
  SerialUSB.begin(9600);
  delay(100);

  SerialUSB.println("Arduino LoRa TX Test!");

 

  while (!rf95.init()) {
    SerialUSB.println("LoRa radio init failed");
    while (1);
  }
  SerialUSB.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    SerialUSB.println("setFrequency failed");
    while (1);
  }
  SerialUSB.print("Set Freq to: "); SerialUSB.println(RF95_FREQ);
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

int16_t packetnum = 0;  // packet counter, we increment per xmission

void loop()
{
  SerialUSB.println("Sending to rf95_server");
  // Send a message to rf95_server
  
  char radiopacket[20] = "Hello World #      ";
  itoa(packetnum++, radiopacket+13, 10);
  SerialUSB.print("Sending "); SerialUSB.println(radiopacket);
  radiopacket[19] = 0;
  
  SerialUSB.println("Sending..."); delay(10);
  rf95.send((uint8_t *)radiopacket, 20);
  delay(1000);
}

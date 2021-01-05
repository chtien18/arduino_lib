#include <SPI.h>
#include <LoRa.h>

int counter = 0;

void setup() {
  SerialUSB.begin(9600);
  //while (!Serial);

  SerialUSB.println("LoRa Sender");

  if (!LoRa.begin(434E6)) {
    SerialUSB.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  SerialUSB.print("Sending packet: ");
  SerialUSB.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);
}

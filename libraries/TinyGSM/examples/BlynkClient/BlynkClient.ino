/**************************************************************
 *
 * For this example, you need to install Blynk library:
 *   https://github.com/blynkkk/blynk-library/releases/latest
 *
 * TinyGSM Getting Started guide:
 *   http://tiny.cc/tiny-gsm-readme
 *
 **************************************************************
 *
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 * Blynk supports many development boards with WiFi, Ethernet,
 * GSM, Bluetooth, BLE, USB/Serial connection methods.
 * See more in Blynk library examples and community forum.
 *
 *                http://www.blynk.io/
 *
 * Change GPRS apm, user, pass, and Blynk auth token to run :)
 **************************************************************/

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space

// Default heartbeat interval for GSM is 60
// If you want override this value, uncomment and set this option:
//#define BLYNK_HEARTBEAT 30

#include <TinyGsmClient.h>
#include <BlynkSimpleSIM800.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your GPRS credentials
// Leave empty, if missing user or pass
char apn[]  = "YourAPN";
char user[] = "";
char pass[] = "";

// Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial1

// or Software Serial on Uno, Nano
//#include <SoftwareSerial.h>
//SoftwareSerial SerialAT(2, 3); // RX, TX

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);

void setup()
{
  // Set console baud rate
  Serial.begin(115200);
  delay(10);

  // Set GSM module baud rate
  SerialAT.begin(115200);
  delay(3000);

  // Restart takes quite some time
  // You can skip it in many cases
  modem.restart();

  Blynk.begin(auth, modem, apn, user, pass);
}

void loop()
{
  Blynk.run();
}


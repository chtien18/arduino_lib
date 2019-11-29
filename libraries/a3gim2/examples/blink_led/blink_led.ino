// 3GIM(V2) sample sketch -- setLED

#include <a3gim2.h>

#define  INTERVAL  1000  // Blink interval
#define baudrate 	9600UL
const int powerPin = 7;     // 3gim power pinIf not using power control, 0 is set.

void setup()
{
  Serial.begin(baudrate);
  delay(100);  // Wait for Start Serial Monitor
  Serial.println("Ready.");

  Serial.print("Initializing.. ");
  if (a3gs.start(powerPin) == 0 && a3gs.begin(0, baudrate) == 0)
    Serial.println("Succeeded.");
  else {
    Serial.println("Failed.");
    Serial.println("Shutdown..");
    a3gs.end();
    a3gs.shutdown();
    return; 
  }
  Serial.println("Blinking..");
}

void loop()
{
  a3gs.setLED(true);
  delay(INTERVAL);
  a3gs.setLED(false);
  delay(INTERVAL);
}

// END

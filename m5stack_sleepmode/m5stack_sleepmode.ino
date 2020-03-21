#include <M5Stack.h>

void setup() {
  M5.begin();

  /*
    Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project
  */
  M5.Power.begin();
  M5.Lcd.setBrightness(200);

 
}

void loop() {

  M5.update();
 
  M5.Lcd.printf("<<Sleep test>>\n");
  
  
  M5.Lcd.printf("Go lightSleep (5s or press buttonA wake up)\n");
  delay(2500);
  // Calling this function will disable the power button
  // To restore power btn please call M5.Power.setPowerBoostKeepOn(false)
  M5.Power.lightSleep(SLEEP_SEC(5000));
  
  
  M5.Lcd.printf("resume.\n\nGo deepSleep (press buttonA wake up) ");
  delay(2500);
  
}

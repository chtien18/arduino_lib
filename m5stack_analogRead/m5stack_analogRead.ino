

#include <M5Stack.h>
// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int analog1 = 35;
const int analog2 = 36;

// variable for storing the potentiometer value
int analog1_Value = 0;
int analog2_Value = 0;

void setup() {
    // initialize the M5Stack object
    pinMode(analog1,INPUT);
    pinMode(analog2,INPUT);
  M5.begin();
  /*
    Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project
  */
  M5.Power.begin();
  M5.Speaker.mute();
// text print
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.printf("Analog1 value:");
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);  
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  M5.Lcd.clear();
  
  analog1_Value = analogRead(analog1);
  Serial.println(analog1_Value);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Analog1 value:");
  M5.Lcd.println(analog1_Value);
  
  analog2_Value = analogRead(analog2);
  Serial.println(analog2_Value);
  M5.Lcd.setCursor(0, 20);
  M5.Lcd.printf("Analog2 value:");
  M5.Lcd.println(analog2_Value);
  
  delay(1000);
  M5.update();
}

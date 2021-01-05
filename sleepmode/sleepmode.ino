#include <avr/sleep.h>
void setup() {
  CLKPR = 0x80;
  CLKPR = 0x01;
}

void loop() {                
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_cpu();
}

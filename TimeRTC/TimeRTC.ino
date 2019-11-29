/*
 * TimeRTC.pde
 * example code illustrating Time library with Real Time Clock.
 * 
 */

#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int set_button=7;
int set_button_status;
int h;
int m;
int s;
int dd;
int mm;
int yy;

void setup()  {
    // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  pinMode(13,OUTPUT);
  pinMode(set_button,INPUT);
  pinMode(6,INPUT);
  digitalWrite(7,HIGH);
  digitalWrite(6,HIGH);
  Serial.begin(9600);
  while (!Serial) ; // wait until Arduino Serial Monitor opens
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if(timeStatus()!= timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");   
}

void loop()
{
  //set time
  set_button_status = digitalRead(set_button);
  if (set_button_status == 0){
    delay(100);
    h=hour();
    m=minute();
    s=second();
    dd=day();
    mm=month();
    yy=year();
    lcd.clear();
    set_button_status = digitalRead(set_button);
      while(set_button_status==1){
        set_button_status = digitalRead(set_button);
        lcd.setCursor(0,0);
        lcd.print("Set hour:");
        lcd.print(h);
        if(digitalRead(6)==0){
          delay(100);
          h++;
          if(h>23) {
            h=0;
            lcd.clear();
            }
        }
        setTime(h,m,s,dd,mm,yy);//set onboard time (hour,minute,second,day,month,year)
        delay(10);
      }
      
      delay(100);
      set_button_status = digitalRead(set_button);
      while(set_button_status==1){
        set_button_status = digitalRead(set_button);
        lcd.setCursor(0,0);
        lcd.print("Set minute:");
        lcd.print(m);
        if(digitalRead(6)==0){
          delay(100);
          m++;
          if(m>60) {
            m=0;
            lcd.clear();
            }
        }
        setTime(h,m,s,dd,mm,yy);//set onboard time (hour,minute,second,day,month,year)
        delay(10);
      }      
    
    RTC.set(now());  //set time to ds1307
    delay(100);
    set_button_status = digitalRead(set_button);
    lcd.clear();
    }
    //end of set_Time
    
  if (timeStatus() == timeSet) {
    digitalClockDisplay();
  } else {
    Serial.println("The time has not been set.  Please run the Time");
    Serial.println("TimeRTCSet example, or DS1307RTC SetTime example.");
    Serial.println();
    delay(4000);
  }
  delay(30);
}

void digitalClockDisplay(){
  // digital clock display of the time
  lcd.setCursor(0,0);
  lcd.print("Time ");
  lcd.print(hour());
  printDigits(minute());
  printDigits(second());
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  lcd.print(":");
  if(digits < 10)
  {
    Serial.print('0');
    lcd.print("0");
  }
  Serial.print(digits);
  lcd.print(digits);
}


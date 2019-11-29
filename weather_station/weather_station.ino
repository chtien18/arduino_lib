/*
 Weather Shield Example
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 16th, 2013
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 Much of this is based on Mike Grusin's USB Weather Board code: https://www.sparkfun.com/products/10586

 This is a more advanced example of how to utilize every aspect of the weather shield. See the basic
 example if you're just getting started.

 This code reads all the various sensors (wind speed, direction, rain gauge, humidity, pressure, light, batt_lvl)
 and reports it over the serial comm port. This can be easily routed to a datalogger (such as OpenLog) or
 a wireless transmitter (such as Electric Imp).

 Measurements are reported once a second but windspeed and rain gauge are tied to interrupts that are
 calculated at each report.

 This example code assumes the GPS module is not used.


  Updated by Joel Bartlett
  03/02/2017
  Removed HTU21D code and replaced with Si7021

 */
#include <SoftwareSerial.h>

SoftwareSerial mySerial(8, 7); // RX, TX

//Hardware pin definitions
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// digital I/O pins
const byte WSPEED = 3;
const byte RAIN = 2;
const byte STAT1 = 13;

// analog I/O pins
const byte WDIR = A0;
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//Global Variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
long lastSecond; //The millis counter to see when a second rolls by
byte seconds; //When it hits 60, increase the current minute
byte seconds_2m; //Keeps track of the "wind speed/dir avg" over last 2 minutes array of data
byte minutes; //Keeps track of where we are in various arrays of data
byte minutes_10m; //Keeps track of where we are in wind gust/dir over last 10 minutes array of data

long lastWindCheck = 0;
volatile long lastWindIRQ = 0;
volatile byte windClicks = 0;

//We need to keep track of the following variables:
//Wind speed/dir each update (no storage)
//Wind gust/dir over the day (no storage)
//Wind speed/dir, avg over 2 minutes (store 1 per second)
//Wind gust/dir over last 10 minutes (store 1 per minute)
//Rain over the past hour (store 1 per minute)
//Total rain over date (store one per day)

byte windspdavg[120]; //120 bytes to keep track of 2 minute average

#define WIND_DIR_AVG_SIZE 120
int winddiravg[WIND_DIR_AVG_SIZE]; //120 ints to keep track of 2 minute average
float windgust_10m[10]; //10 floats to keep track of 10 minute max
int windgustdirection_10m[10]; //10 ints to keep track of 10 minute max
volatile float rainHour[10]; //60 floating numbers to keep track of 60 minutes of rain

//These are all the weather values that wunderground expects:
int winddir = 0; // [0-360 instantaneous wind direction]
float windspeedmph = 0; // [mph instantaneous wind speed]
float windgustmph = 0; // [mph current wind gust, using software specific time period]
int windgustdir = 0; // [0-360 using software specific time period]
float windspdmph_avg2m = 0; // [mph 2 minute average wind speed mph]
int winddir_avg2m = 0; // [0-360 2 minute average wind direction]
float windgustmph_10m = 0; // [mph past 10 minutes wind gust mph ]
int windgustdir_10m = 0; // [0-360 past 10 minutes wind gust direction]

float rainin = 0; // [rain inches over the past hour)] -- the accumulated rainfall in the past 60 min
volatile float dailyrainin = 0; // [rain inches so far today in local time]

// volatiles are subject to modification by IRQs
volatile unsigned long raintime, rainlast, raininterval, rain;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//Interrupt routines (these are called by the hardware interrupts, not by the main code)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void rainIRQ()
// Count rain gauge bucket tips as they occur
// Activated by the magnet and reed switch in the rain gauge, attached to input D2
{
    raintime = millis(); // grab current time
    raininterval = raintime - rainlast; // calculate interval between this and last event

    if (raininterval > 10) // ignore switch-bounce glitches less than 10mS after initial edge
    {
        dailyrainin += 0.011; //Each dump is 0.011" of water
        rainHour[minutes] += 0.011; //Increase this minute's amount of rain

        rainlast = raintime; // set up for next event
    }
}

void wspeedIRQ()
// Activated by the magnet in the anemometer (2 ticks per rotation), attached to input D3
{
    if (millis() - lastWindIRQ > 10) // Ignore switch-bounce glitches less than 10ms (142MPH max reading) after the reed switch closes
    {
        lastWindIRQ = millis(); //Grab the current time
        windClicks++; //There is 1.492MPH for each click per second.
    }
}


void setup()
{
    Serial.begin(9600);
   
    Serial.println("Weather Shield Example");
    pinMode(9,OUTPUT);
    pinMode(STAT1, OUTPUT); //Status LED Blue
    mySerial.begin(9600);

    pinMode(WSPEED, INPUT_PULLUP); // input from wind meters windspeed sensor
    pinMode(RAIN, INPUT_PULLUP); // input from wind meters rain gauge sensor

    seconds = 0;
    lastSecond = millis();

    // attach external interrupt pins to IRQ functions
    attachInterrupt(digitalPinToInterrupt(RAIN), rainIRQ, FALLING);
    attachInterrupt(digitalPinToInterrupt(WSPEED), wspeedIRQ, FALLING);
    

    // turn on interrupts
    interrupts();
    digitalWrite(9,LOW);
    Serial.println("Weather Shield online!");

}

void loop()
{
    //Keep track of which minute it is
  if(millis() - lastSecond >= 1000)
    {
        digitalWrite(STAT1, HIGH); //Blink stat LED

    lastSecond += 1000;

        //Take a speed and direction reading every second for 2 minute average
        if(++seconds_2m > 119) seconds_2m = 0;

        //Calc the wind speed and direction every second for 120 second to get 2 minute average
        float currentSpeed = get_wind_speed();
        //float currentSpeed = random(5); //For testing
        int currentDirection = get_wind_direction();
        windspdavg[seconds_2m] = (int)currentSpeed;
        winddiravg[seconds_2m] = currentDirection;
        //if(seconds_2m % 10 == 0) displayArrays(); //For testing

        //Check to see if this is a gust for the minute
        if(currentSpeed > windgust_10m[minutes_10m])
        {
            windgust_10m[minutes_10m] = currentSpeed;
            windgustdirection_10m[minutes_10m] = currentDirection;
        }

        //Check to see if this is a gust for the day
        if(currentSpeed > windgustmph)
        {
            windgustmph = currentSpeed;
            windgustdir = currentDirection;
        }

        if(++seconds > 59)
        {
            
            seconds = 0;

            if(++minutes > 59) minutes = 0;
            if(++minutes_10m > 9) //default 9
            {
              //Report all readings 10 minutes
              printWeather();
              SubmitHttpRequest();
              minutes_10m=0;
               for(int i = 0 ; i < 10 ; i++) rainHour[i]=0;
            }
            rainHour[minutes] = 0; //Zero out this minute's rainfall amount
            windgust_10m[minutes_10m] = 0; //Zero out this minute's gust
        }   

        digitalWrite(STAT1, LOW); //Turn off stat LED
    }

  delay(100);
}

//Calculates each of the variables that wunderground is expecting
void calcWeather()
{
    //Calc winddir
    winddir = get_wind_direction();

    //Calc windspeed
    //windspeedmph = get_wind_speed(); //This is calculated in the main loop

    //Calc windgustmph
    //Calc windgustdir
    //These are calculated in the main loop

    //Calc windspdmph_avg2m
    float temp = 0;
    for(int i = 0 ; i < 120 ; i++)
        temp += windspdavg[i];
    temp /= 120.0;
    windspdmph_avg2m = temp;

    //Calc winddir_avg2m, Wind Direction
    //You can't just take the average. Google "mean of circular quantities" for more info
    //We will use the Mitsuta method because it doesn't require trig functions
    //And because it sounds cool.
    //Based on: http://abelian.org/vlf/bearings.html
    //Based on: http://stackoverflow.com/questions/1813483/averaging-angles-again
    long sum = winddiravg[0];
    int D = winddiravg[0];
    for(int i = 1 ; i < WIND_DIR_AVG_SIZE ; i++)
    {
        int delta = winddiravg[i] - D;

        if(delta < -180)
            D += delta + 360;
        else if(delta > 180)
            D += delta - 360;
        else
            D += delta;

        sum += D;
    }
    winddir_avg2m = sum / WIND_DIR_AVG_SIZE;
    if(winddir_avg2m >= 360) winddir_avg2m -= 360;
    if(winddir_avg2m < 0) winddir_avg2m += 360;

    //Calc windgustmph_10m
    //Calc windgustdir_10m
    //Find the largest windgust in the last 10 minutes
    windgustmph_10m = 0;
    windgustdir_10m = 0;
    //Step through the 10 minutes
    for(int i = 0; i < 10 ; i++)
    {
        if(windgust_10m[i] > windgustmph_10m)
        {
            windgustmph_10m = windgust_10m[i];
            windgustdir_10m = windgustdirection_10m[i];
        }
    }

    //Total rainfall for the day is calculated within the interrupt
    //Calculate amount of rainfall for the last 10 minutes
    rainin = 0;
    for(int i = 0 ; i < 10 ; i++)
        rainin += rainHour[i];
}

//Returns the instataneous wind speed
float get_wind_speed()
{
    float deltaTime = millis() - lastWindCheck; //750ms

    deltaTime /= 1000.0; //Covert to seconds

    float windSpeed = (float)windClicks / deltaTime; //3 / 0.750s = 4

    windClicks = 0; //Reset and start watching for new wind
    lastWindCheck = millis();

    windSpeed *= 1.492; //4 * 1.492 = 5.968MPH

    /* Serial.println();
     Serial.print("Windspeed:");
     Serial.println(windSpeed);*/

    return(windSpeed);
}

//Read the wind direction sensor, return heading in degrees
int get_wind_direction()
{
    unsigned int adc;

    adc = analogRead(WDIR); // get the current reading from the sensor

    // The following table is ADC readings for the wind direction sensor output, sorted from low to high.
    // Each threshold is the midpoint between adjacent headings. The output is degrees for that ADC reading.
    // Note that these are not in compass degree order! See Weather Meters datasheet for more information.

    if (adc < 65) return (113);
    if (adc < 84) return (68);
    if (adc < 92) return (90);
    if (adc < 125) return (158);
    if (adc < 186) return (135);
    if (adc < 245) return (203);
    if (adc < 287) return (180);
    if (adc < 405) return (23);
    if (adc < 465) return (45);
    if (adc < 601) return (248);
    if (adc < 633) return (225);
    if (adc < 707) return (338);
    if (adc < 790) return (0);
    if (adc < 830) return (293);
    if (adc < 890) return (315);
    if (adc < 949) return (270);
    return (-1); // error, disconnected?
}


//Prints the various variables directly to the port
//I don't like the way this function is written but Arduino doesn't support floats under sprintf
void printWeather()
{
    calcWeather(); //Go calc all the various sensors
    
    Serial.println();
    Serial.print("$,winddir=");
    Serial.print(winddir);
    Serial.print(",windspeedmph=");
    Serial.print(windspeedmph, 1);
    Serial.print(",windgustmph=");
    Serial.print(windgustmph, 1);
    Serial.print(",windgustdir=");
    Serial.print(windgustdir);
    Serial.print(",windspdmph_avg2m=");
    Serial.print(windspdmph_avg2m, 1);/////////////////
    Serial.print(",winddir_avg2m=");
    Serial.print(winddir_avg2m);//////////////////////
    Serial.print(",windgustmph_10m=");
    Serial.print(windgustmph_10m, 1);///////////////////
    Serial.print(",windgustdir_10m=");
    Serial.print(windgustdir_10m);
    Serial.print(",rainin=");
    Serial.print(rainin, 2);/////////////////////////////
    Serial.print(",dailyrainin=");
    Serial.print(dailyrainin, 2);
    Serial.println("#");
}


// SubmitHttpRequest()
//
// Note: the time of the delays are very important
void SubmitHttpRequest()
{
 
   digitalWrite(9,HIGH);
   delay(2000);
   digitalWrite(9,LOW);
   delay(1000);
  // digitalWrite(9,HIGH);
  // digitalWrite(13,HIGH);
   delay(15000);
   mySerial.println("AT");
   delay(1000);
   ShowSerialData();
  // Query signal strength of device
  mySerial.println("AT+CSQ");
  delay(1000);

  ShowSerialData();

  mySerial.println("AT+COPS?");
  delay(1000);

  ShowSerialData();

  // Check the status of Packet service attach. '0' implies device is not attached and '1' implies device is attached.
  mySerial.println("AT+CGATT?");
  delay(1000);

  ShowSerialData();

  // Set the SAPBR, the connection type is using gprs
  mySerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(500);

  ShowSerialData();

  // Set the APN
  mySerial.println("AT+SAPBR=3,1,\"APN\",\"e-connect\"");
  delay(2000);

  ShowSerialData();

  // Set the SAPBR, for detail you can refer to the AT command manual
  mySerial.println("AT+SAPBR=1,1");
  delay(1000);

  ShowSerialData();

  // Init the HTTP request
  mySerial.println("AT+HTTPINIT");

  delay(2000);
  ShowSerialData();

  // Set HTTP params, the second param is the website to request
  //mySerial.println("AT+HTTPPARA=\"URL\",\"api.thingspeak.com/update?api_key=G4SDI9TYWFKHS4N1&field1=" + String(quatI) + "&field2=" + String(quatJ) + "&field3=" + String(quatK)+ "&field4=" + String(quatReal)+ "&field5=" + String(x)+ "&field6=" + String(y)+ "&field7=" + String(z)+"\"");
  //delay(1000); //--->for device1
  mySerial.println("AT+HTTPPARA=\"URL\",\"api.thingspeak.com/update?api_key=9UA7VGZYL6HM450I&field1=" + String(windspdmph_avg2m) + "&field2=" + String(winddir_avg2m) + "&field3=" + String(windgustmph_10m)+ "&field4=" + String(rainin)+"\"");
  delay(1000);
  
  ShowSerialData();

  //Set the context ID
  mySerial.println("AT+HTTPPARA=\"CID\",1");
  delay(1000);

  ShowSerialData();

  // Submit the request
  mySerial.println("AT+HTTPACTION=0");
  // The delay is very important, the delay time is base on the
  // return time from the website, if the return data is very
  // large, the time required might be longer.
  delay(2000);

  ShowSerialData();

  // Read the data from the accessed website
  mySerial.println("AT+HTTPREAD");
  delay(5000);

  ShowSerialData();

  // Close the HTTP connection and display the data
  mySerial.println("AT+HTTPTERM");
  delay(1000);
  mySerial.println("AT+CPOWD=0");
  delay(1000);
 // digitalWrite(9,LOW);
   // digitalWrite(13,LOW);
   // delay(500);
}


void ShowSerialData()
{
  while(mySerial.available()!=0)
    //mySerial.read();
    Serial.write(mySerial.read());
}


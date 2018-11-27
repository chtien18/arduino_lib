//Hardware pin definitions
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// digital I/O pins
const byte RAIN = 2;
const byte STAT1 = 13;

float rainin = 0; // [rain inches over the past hour)] -- the accumulated rainfall in the past 60 min
volatile float dailyrainin = 0; // [rain inches so far today in local time]
volatile float rainHour[60]; //60 floating numbers to keep track of 60 minutes of rain

//Global Variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
long lastSecond; //The millis counter to see when a second rolls by
byte seconds; //When it hits 60, increase the current minute
byte seconds_2m; //Keeps track of the "wind speed/dir avg" over last 2 minutes array of data
byte minutes; //Keeps track of where we are in various arrays of data
byte minutes_10m; //Keeps track of where we are in wind gust/dir over last 10 minutes array of data


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
        Serial.println(dailyrainin,3);
        rainlast = raintime; // set up for next event
    }

}

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.println("Weather Shield Example");
    pinMode(RAIN,INPUT_PULLUP);
    pinMode(STAT1, OUTPUT); //Status LED Blue
//    pinMode(STAT2, OUTPUT); //Status LED Green

    seconds = 0;
    lastSecond = millis();

    // attach external interrupt pins to IRQ functions
   // attachInterrupt(2, rainIRQ, FALLING);
  //  attachInterrupt(1, wspeedIRQ, FALLING);
    attachInterrupt(digitalPinToInterrupt(RAIN), rainIRQ, FALLING);
    // turn on interrupts
    interrupts();
    Serial.println("Weather Shield online!");

}

void loop() {
  if(millis() - lastSecond >= 1000)
    {
    digitalWrite(STAT1, HIGH); //Blink stat LED
    lastSecond += 1000;
}

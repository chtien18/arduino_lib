#include <Arduino.h>
#include <U8g2lib.h>
#include <SDI12.h>

#define SERIAL_BAUD 9600  // The baud rate for the output serial port
#define DATA_PIN 3         // The pin of the SDI-12 data bus
#define POWER_PIN 7       // The sensor power pin (or -1 if not switching power)
#define SENSOR_ADDRESS 0

#define Serial SerialUSB

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// Define the SDI-12 bus
SDI12 mySDI12(DATA_PIN);

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


//****************Print Buffer to screen************
void printBufferToScreen(){
  String mybuffer = "";
  mySDI12.read(); // consume address
  while(mySDI12.available()){
    char c = mySDI12.read();
    if(c == '+'){
      mybuffer += ',';
    }
    else if ((c != '\n') && (c != '\r')) {
      mybuffer += c;
    }
    delay(100);
  }
 Serial.println(mybuffer);
 float values[]={};
 int j=0;
 
 int len=mybuffer.length()+1;
    char data[len];
    mybuffer.toCharArray(data,len);
    for (int i=0;i<len;i++ )
    {
      Serial.print(data[i]);
      delay(100);
      if(data[i]==','){
       Serial.println();
        }
     }
     Serial.println("");
    
    int k=15;    
    u8g2.clearBuffer();
    delay(20);
    u8g2.firstPage();
    do{
    u8g2.setCursor(0, 15);
    u8g2.println("Sensor's data");
    u8g2.setCursor(0, 30);
    for (int i=0;i<len;i++ )
    {  
      if(data[i]==',' || data[i]==':') 
      {
        data[i]=':';
        k=k+15;
        u8g2.setCursor(0,k);
        u8g2.print("Value"+String(k/15-1));
      }
       u8g2.print(data[i]);
       
     }

    k=15;
    } while ( u8g2.nextPage() );
    delay(100);
}
//--------------------------------------------------------
// gets identification information from a sensor, and prints it to the serial port
// expects a character between '0'-'9', 'a'-'z', or 'A'-'Z'.
void printInfo(char i){
  String command = "";
  command += (char) i;
  command += "I!";
  mySDI12.sendCommand(command);
   Serial.print(">>>");
   Serial.println(command);
  delay(30);

  printBufferToScreen();
}
//------------------------------------------------------------------------

//******************Take measurement SDI12 sensor******************************
void takeMeasurement(char i){
  Serial.println("taking measure.....");
  digitalWrite(POWER_PIN, HIGH);
  delay(10000);
  String command = "";
  command += i;
  command += "M!"; // SDI-12 measurement command format  [address]['M'][!]
  mySDI12.sendCommand(command);
  delay(30);

  // wait for acknowlegement with format [address][ttt (3 char, seconds)][number of measurments available, 0-9]
  String sdiResponse = "";
  delay(30);
  while (mySDI12.available())  // build response string
  {
    char c = mySDI12.read();
    if ((c != '\n') && (c != '\r'))
    {
      sdiResponse += c;
      delay(5);
    }
  }
  mySDI12.clearBuffer();

  // find out how long we have to wait (in seconds).
  uint8_t wait = 0;
  wait = sdiResponse.substring(1,4).toInt();

  // Set up the number of results to expect
  // int numMeasurements =  sdiResponse.substring(4,5).toInt();

  unsigned long timerStart = millis();
  while((millis() - timerStart) < (1000 * wait)){
    if(mySDI12.available())  // sensor can interrupt us to let us know it is done early
    {
      mySDI12.clearBuffer();
      break;
    }
  }
  // Wait for anything else and clear it out
  delay(30);
  mySDI12.clearBuffer();

  // in this example we will only take the 'DO' measurement
  command = "";
  command += i;
  command += "D0!"; // SDI-12 command to get data [address][D][dataOption][!]
  mySDI12.sendCommand(command);
  while(!mySDI12.available()>1); // wait for acknowlegement
  delay(300); // let the data transfer
  printBufferToScreen();
  mySDI12.clearBuffer();
  delay(300);
 digitalWrite(POWER_PIN, LOW);//Turn off sensor's power line
  delay(200);
}
//----------------------------------------------------


void setup(void) {
  Serial.begin(SERIAL_BAUD);
  delay(10);
  pinMode(POWER_PIN,OUTPUT);
  digitalWrite(POWER_PIN, HIGH);
  u8g2.begin();
  u8g2.enableUTF8Print();   // enable UTF8 support for the Arduino print() function
  delay(100);
  Serial.println("Opening SDI-12 bus...");
  u8g2.setFont(u8g2_font_t0_15b_tf);  
  u8g2.setFontDirection(0);
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 15);
    u8g2.print("Opening SDI-12 bus...");
  } while ( u8g2.nextPage() );
  delay(5000);
  
  mySDI12.begin();
  delay(500); // allow things to settle

  // Power the sensors;
  if(POWER_PIN > 0){
    Serial.println("Powering up sensors...");
    pinMode(POWER_PIN, OUTPUT);
    digitalWrite(POWER_PIN, HIGH);
    delay(200);
  }

  printInfo('0');
  
}

void loop(void) {

  takeMeasurement('0');
  delay(10000);
 
}

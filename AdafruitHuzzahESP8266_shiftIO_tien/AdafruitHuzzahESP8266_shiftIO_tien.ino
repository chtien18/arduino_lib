// This example uses an Adafruit Huzzah ESP8266
// to connect to shiftr.io.
//
// You can check on your device after a successful
// connection here: https://shiftr.io/try.
//
// by Joël Gähwiler
// https://github.com/256dpi/arduino-mqtt

#include <ESP8266WiFi.h>
#include <MQTTClient.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <Adafruit_BMP085_U.h>


const char *ssid = "agrinode";
const char *pass = "12345678";

float tsl2561_luminosity=0;
float bmp180_pressure=0;

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" lux");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" lux");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" lux");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);

  bmp.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" hPa");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" hPa");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" hPa");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}
/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2561
*/
/**************************************************************************/
void configureSensor(void)
{
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

  /* Update these values depending on what you've set above! */  
  Serial.println("------------------------------------");
  Serial.print  ("Gain:         "); Serial.println("Auto");
  Serial.print  ("Timing:       "); Serial.println("13 ms");
  Serial.println("------------------------------------");
}




void connect(); // <- predefine connect() for setup()

void setup() {
   Serial.begin(9600);
   Serial.println("Light Sensor Test"); Serial.println("");
  
    /* Initialise the sensor */
   if(!tsl.begin())
   {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
    }

    Serial.println("Pressure Sensor Test"); Serial.println("");
    
    /* Initialise the sensor */
    if(!bmp.begin())
    {
      /* There was a problem detecting the BMP085 ... check your connections */
      Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
      while(1);
    }
  /* Display some basic information on this sensor */
   displaySensorDetails();
  
  /* Setup the sensor gain and integration time */
   configureSensor();
  
  /* We're ready to go! */
  Serial.println("");
  
  WiFi.begin(ssid, pass);
  client.begin("172.16.1.1", net);

  connect();
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "chtien18", "iotsample-arduino")) {  //client, user, pass
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("node01");
  // client.unsubscribe("/example");
}

void loop() {
  client.loop();
  delay(10); // <- fixes some issues with WiFi stability

  if(!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
 // if(millis() - lastMillis > 5000) {
 //   lastMillis = millis();
 //   client.publish("/hello", "world");
 // }
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();

  if(payload[0]=='1'){
    
    tsl2561_mesurement();
    bmp180_mesurement();
    
    String json=buildJson();
    char jsonStr[200];
    json.toCharArray(jsonStr,200);
    Serial.print(jsonStr);
    client.publish("node01_data",jsonStr);
    }
}

void tsl2561_mesurement(){
      /* Get a new sensor event */ 
      sensors_event_t event;
      tsl.getEvent(&event);
     
      /* Display the results (light is measured in lux) */
      if (event.light)
      {
        Serial.print(event.light); Serial.println(" lux");
        tsl2561_luminosity = event.light;
      }
      else
      {
        /* If event.light = 0 lux the sensor is probably saturated
           and no reliable data could be generated! */
        Serial.println("Sensor overload");
      }
      delay(250);
  }

  void bmp180_mesurement(){
    /* Get a new sensor event */ 
  sensors_event_t event;
  bmp.getEvent(&event);
 
  /* Display the results (barometric pressure is measure in hPa) */
  if (event.pressure)
  {
    /* Display atmospheric pressue in hPa */
    Serial.print("Pressure:    ");
    Serial.print(event.pressure);
    Serial.println(" hPa");
    bmp180_pressure = event.pressure;
    
    /* Calculating altitude with reasonable accuracy requires pressure    *
     * sea level pressure for your position at the moment the data is     *
     * converted, as well as the ambient temperature in degress           *
     * celcius.  If you don't have these values, a 'generic' value of     *
     * 1013.25 hPa can be used (defined as SENSORS_PRESSURE_SEALEVELHPA   *
     * in sensors.h), but this isn't ideal and will give variable         *
     * results from one day to the next.                                  *
     *                                                                    *
     * You can usually find the current SLP value by looking at weather   *
     * websites or from environmental information centers near any major  *
     * airport.                                                           *
     *                                                                    *
     * For example, for Paris, France you can check the current mean      *
     * pressure and sea level at: http://bit.ly/16Au8ol                   */
     
    /* First we get the current temperature from the BMP085 */
    float temperature;
    bmp.getTemperature(&temperature);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    /* Then convert the atmospheric pressure, and SLP to altitude         */
    /* Update this next line with the current SLP for better results      */
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    Serial.print("Altitude:    "); 
    Serial.print(bmp.pressureToAltitude(seaLevelPressure,
                                        event.pressure)); 
    Serial.println(" m");
    Serial.println("");
  }
  else
  {
    Serial.println("Sensor error");
  }
  delay(1000);    
  }

String buildJson() {
  String data = "{";

  data+="\"ID\": \"Node01\",";

  data+="\"Luminosity\": ";
  data+=(float)tsl2561_luminosity;
  data+= ",";
  //data+="\n";
  data+="\"Pressure\": ";
  data+=(float)bmp180_pressure;
 //data+= ",";
  //data+="\n";
 
  data+="}";
  data+="\n";
  //data+="}";
  return data;
}

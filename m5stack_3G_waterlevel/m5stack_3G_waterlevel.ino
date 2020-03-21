#include <M5Stack.h>

#define TINY_GSM_MODEM_UBLOX
#include <TinyGsmClient.h>

TinyGsm modem(Serial2); /* 3G board modem */
TinyGsmClient ctx(modem);

const int analog1 = 35;
const int analog2 = 36;

int analog1_Value = 0;
int analog2_Value = 0;

void init_network(){
  M5.Lcd.print(F("modem.restart()"));
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  modem.restart();
  M5.Lcd.println(F("done"));

  M5.Lcd.print(F("getModemInfo:"));
  String modemInfo = modem.getModemInfo();
  M5.Lcd.println(modemInfo);

  M5.Lcd.print(F("waitForNetwork()"));
  while (!modem.waitForNetwork()) M5.Lcd.print(".");
  M5.Lcd.println(F("Ok"));

  M5.Lcd.print(F("gprsConnect(mobifone)"));
  modem.gprsConnect("m-wap", "mms", "mms");
  M5.Lcd.println(F("done"));

  M5.Lcd.print(F("isNetworkConnected()"));
  while (!modem.isNetworkConnected()) M5.Lcd.print(".");
  M5.Lcd.println(F("Ok"));

  M5.Lcd.print(F("My IP addr: "));
  IPAddress ipaddr = modem.localIP();
  M5.Lcd.print(ipaddr);
  delay(2000);
  }
void setup() {
  Serial.begin(115200);
  
  pinMode(analog1,INPUT);
  pinMode(analog2,INPUT);
  
  M5.begin();
  M5.Power.begin();
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.println(F("M5Stack + 3G Module"));
  delay(1000);
  getdata();
  init_network();
}

void loop() {
  M5.update();

  M5.Lcd.clear(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println(F("api.thingspeak.com"));

  /* HTTP GET example */
  if (!ctx.connect("api.thingspeak.com", 80)) {
    Serial.println(F("Connect failed."));
    init_network();
    return;
  }
  Serial.println(F("connected."));
  
  /*Get sensor datata*/
  String tsdata=getdata();
  
  /* send request */
  ctx.println("GET /update?api_key=X0OIASPTMFKF4PFG" + tsdata + " HTTP/1.0");
  ctx.println("Host: api.thingspeak.com");
  ctx.println();
  Serial.println("sent.");

  /* receive response */
  while (ctx.connected()) {
    String line = ctx.readStringUntil('\n');
    Serial.println(line);
    if (line == "\r") {
      Serial.println("headers received.");
      break;
    }
  }
  char buf[1 * 1024] = {0};
  ctx.readBytes(buf, sizeof(buf)); /* body */
  ctx.stop();
  M5.Lcd.println(buf);

  for(int i=0;i<60;i++)//sleep for 60 minutes
  M5.Power.lightSleep(SLEEP_SEC(60));//sleep for one minute
}

String getdata()
{
  String data_str="";
  M5.Lcd.println("Get data...");
  analog1_Value = analogRead(analog1);
  Serial.println(analog1_Value);
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.printf("Analog1 value:");
  M5.Lcd.println(analog1_Value);
  
  analog2_Value = analogRead(analog2);
  Serial.println(analog2_Value);
  M5.Lcd.setCursor(0, 20);
  M5.Lcd.printf("Analog2 value:");
  M5.Lcd.println(analog2_Value);

  data_str +="&field1=";
  
  data_str +=analog1_Value;
  data_str +="&field2=";
  data_str +=analog2_Value;
  
  Serial.println(data_str);
  M5.Lcd.setCursor(0, 40);
  M5.Lcd.printf("Data:");
  M5.Lcd.println(data_str);
  return data_str;
  }

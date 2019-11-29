#include <M5Stack.h>

#define TINY_GSM_MODEM_UBLOX
#include <TinyGsmClient.h>

TinyGsm modem(Serial2); /* 3G board modem */
TinyGsmClient ctx(modem);

void setup() {
  Serial.begin(115200);
  M5.begin();
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.println(F("M5Stack + 3G Module"));

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

void loop() {
  M5.update();

  M5.Lcd.clear(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println(F("World Clock from worldtimeapi.org"));

  /* HTTP GET example */
  if (!ctx.connect("worldtimeapi.org", 80)) {
    Serial.println(F("Connect failed."));
    return;
  }
  Serial.println(F("connected."));

  /* send request */
  ctx.println("GET /api/timezone/Asia/Tokyo.txt HTTP/1.0");
  ctx.println("Host: worldtimeapi.org");
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

  delay(1000 * 10);
}

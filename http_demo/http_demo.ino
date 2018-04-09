#include "Http.h"

unsigned int RST_PIN = 12;

void setup(){
Serial.begin(9600);
delay(100);
HTTP http(9600, RST_PIN);
http.configureBearer("movistar.es");

http.connect();



   


char response[256];
Result result = http.get("your.api.com", response);

Serial.println(response);

http.disconnect();

delay(30000);
}
void loop(){
}

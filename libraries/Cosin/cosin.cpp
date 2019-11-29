 /* Copyright (c) 2017 Cosin technology inc.
 * Author        :   Tien
 * Create Time   :   Oct 2017
 * Change Log    :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "cosin.h"
#include "HardwareSerial.h"


#define serialSIM800 Serial1

COSIN::COSIN(int baud)
{
    value = baud;
    pinMode(7,OUTPUT);
    digitalWrite(7,HIGH);
    delay(13000);
    serialSIM800.begin(value);
}

void COSIN::init(void)
{
    while(serialSIM800.available()) serialSIM800.read();
    serialSIM800.write("AT\r\n");
    delay(1000);
    serialSIM800.write("AT\r\n");
    delay(2000);
    serialSIM800.write("AT\r\n");
    delay(2000);
}
void COSIN::configureBearer(void)
{

    serialSIM800.write("AT+CREG?");
    Serial.write("AT+CREG?");
    delay(10000);
    serialSIM800.write("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n");
    delay(10000);
    serialSIM800.write("AT+SAPBR=3,1,\"APN\",\"v-connect\"\r\n");
    delay(10000);
    serialSIM800.write("AT+SAPBR=1,1\r\n");
    delay(10000);
    serialSIM800.write("AT+HTTPINIT\r\n");
    delay(10000);
    serialSIM800.write("AT+HTTPPARA=\"CID\",1\r\n");
    delay(10000);
    serialSIM800.write("AT+HTTPPARA=\"URL\",\"cosin.herokuapp.com/chtien18/write/val1=9&val2=10\"\r\n");
    delay(10000);
    serialSIM800.write("AT+SAPBR=0,1\r\n");
    Serial.write("AT+SAPBR=0,1\r\n");
    
}

void COSIN::serialDebug(void)
{
    while(1) {
        if(serialSIM800.available()){
            Serial.write(serialSIM800.read());
        }
        if(Serial.available()){     
            serialSIM800.write(Serial.read()); 
        }
    }
}

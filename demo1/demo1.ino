void setup() {
  pinMode(13,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 if (Serial.available()){
  int ch = Serial.read();

  if (ch==0){
    digitalWrite(13,HIGH);
    delay(100);
    }

  }
  
}


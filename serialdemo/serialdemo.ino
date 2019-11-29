int value =65;
void setup() {
 Serial.begin(9600); // put your setup code here, to run once:

}

void loop() {
  Serial.write(value);
  delay(1000);
  // put your main code here, to run repeatedly:

}

void setup() {
  Serial.begin(115200);
  float degC=31.3;
  float degF;
  degF = degC*9/5+32;
  
  Serial.print("degF=");
  Serial.println(degF);
}

void loop() {

}

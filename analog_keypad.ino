void setup() {
  Serial.begin(9600);

}

void loop() {
  int v = analogRead(A0);
  int v1 = analogRead(A1);
  Serial.println(v1-v);
  Serial.println(v1);
  Serial.println();
  delay(300);
 
  

}

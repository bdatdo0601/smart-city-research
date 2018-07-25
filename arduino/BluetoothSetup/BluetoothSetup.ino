void setup() {
  // put your main code here, to run repeatedly
  delay(5000);
  Serial.print("AT");
  delay(5000);
  Serial.println();
  if (Serial.available()) {
    while (Serial.available()) {
      Serial.write(Serial.read());
    }
  }
  Serial.println();
  delay(5000);
  Serial.print("AT+NAME=TESTVEC2");
  delay(5000);
  Serial.println();
  if (Serial.available()) {
    while (Serial.available()) {
      Serial.write(Serial.read());
    }
  }
  Serial.println();
  delay(5000);
  Serial.print("AT+RX");
  delay(5000);
  Serial.println();
  if (Serial.available()) {
    while (Serial.available()) {
      Serial.write(Serial.read());
    }
  }
  Serial.println();
  delay(5000);
  Serial.println("BT INIT DONE!");
}

void loop(){}

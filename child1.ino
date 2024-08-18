// sender
//enter a string in serial monitor of child1
//and receive it on serial monitor of child2
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(2400);
}
void loop() {
  Wire.beginTransmission(1);
  while (Serial.available()) {
    char c = Serial.read();
    Serial.print(c);
    Wire.write(c);
  }
  Wire.endTransmission(1);
}

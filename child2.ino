// reciever

//enter a string in serial monitor of child1
//and receive it on serial monitor of child2

#include <SoftwareSerial.h>
#define RX 2
#define TX 3

SoftwareSerial master_serial(RX, TX);

void setup() {
  pinMode(TX, OUTPUT);
  pinMode(RX, INPUT);
  Serial.begin(2400);
  master_serial.begin(2400);
}

void loop() {
  while (master_serial.available()) {
    char c = master_serial.read();
    Serial.print(c);
  }
}
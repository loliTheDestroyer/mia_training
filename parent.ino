// MASTER
//enter a string in serial monitor of child1
//and receive it on serial monitor of child2
#include <SoftwareSerial.h>
#include <Wire.h>
#define RX 2
#define TX 3

SoftwareSerial receiver_serial(RX, TX);

void setup() {
  pinMode(TX, OUTPUT);
  pinMode(RX, INPUT);
  digitalWrite(TX, HIGH);
  Serial.begin(2400);
  Wire.begin(1);
  receiver_serial.begin(2400);
  Wire.onReceive(print_message);
}

void print_message(int a) {
  while (Wire.available()) {
    char c = Wire.read();
    receiver_serial.print(c);
    Serial.print(c);
  }
}

void loop() {
}
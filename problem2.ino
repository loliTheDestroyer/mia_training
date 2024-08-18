#include <Servo.h>
#define SERVO 6

Servo defuser;
void setup() {
  pinMode(SERVO, OUTPUT);
  analogWrite(SERVO, 0);
  defuser.attach(6);
  defuser.write(0);
}

void loop() {

  defuser.write(60);
  delay(2000);
  defuser.write(0);
  delay(5000);
}
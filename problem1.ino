
void setup() {
  pinMode(A0, INPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(A0));
  if (analogRead(A0) >= 500) {
    tone(11, 500);
    delay(50);
  }
  tone(11, 50);
  delay(500);
  noTone(11);
  delay(500);
}
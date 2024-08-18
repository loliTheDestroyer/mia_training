const int trig_pin = 10;
const int echo_pin = 9;

int duration;
float distance;
void setup() {
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  Serial.begin(9600);
}
void loop() {
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  duration = pulseIn(echo_pin, HIGH);
  distance = duration * 0.0343 / 2;
  if (distance < 300) {
    Serial.print("Object detected in range ");
    Serial.println(distance);
  }
  delay(1000);
}
//The robot will go around the object once detected
//It will navigate autonomously through the whole map defusing bombs


#include <Servo.h>

#define R1 4
#define R2 5
#define L1 2
#define L2 3
#define SERVO 9
#define PHOTO_RESISTOR A0
#define BUZZER 6
#define TRIG 7
#define FRONT_SENSOR 8
#define RIGHT_SENSOR 10
#define LEFT_SENSOR 11


enum Facing { POSX = 0,
              NEGY,
              NEGX,
              POSY };
enum Direction { FORWARDS = 1,
                 BACKWARDS = -1 };
enum Rotation { CW = 1,
                CCW = -1 };
enum Sensor { right_sensor = 10,
              left_sensor = 11,
              front_sensor = 8 };
struct Position {
  int x;
  int y;
  Facing facing;
};
Servo defuser;


void move_motors(const Direction dir_l, const Direction dir_r) {
  int l1 = dir_l == FORWARDS ? HIGH : LOW;
  int r1 = dir_r == FORWARDS ? HIGH : LOW;
  digitalWrite(L1, l1);
  digitalWrite(L2, !l1);
  digitalWrite(R1, r1);
  digitalWrite(R2, !r1);

  delay(1000);

  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);
}
Position move(Position pos, const Direction dir) {
  switch (pos.facing) {
    case POSX: pos.x += dir; break;
    case POSY: pos.y += dir; break;
    case NEGX: pos.x -= dir; break;
    case NEGY: pos.y -= dir; break;
  }
  move_motors(dir, dir);
  print_pos(pos);
  return pos;
}
Position rot(const Position pos, const Rotation rot) {
  // POSX(0) <-> NEGY(1) <-> NEGX(2) <-> POSY(3) (mod 4) (+ 4 to handle neg numbers)
  const int facing = (pos.facing + rot + 4) % 4;

  if (rot == CW)
    move_motors(FORWARDS, BACKWARDS);
  else
    move_motors(BACKWARDS, FORWARDS);

  return { pos.x, pos.y, static_cast<Facing>(facing) };
}
void print_pos(const Position pos) {
  Serial.print("(");
  Serial.print(pos.x);
  Serial.print(", ");
  Serial.print(pos.y);
  Serial.println(")");
}
Position avoid_obstacle(Position pos) {
  Sensor sensor = pos.facing == POSX ? right_sensor : left_sensor;
  Rotation rotation = pos.facing == POSX ? CCW : CW;
  Rotation opposite_rot = pos.facing == POSX ? CW : CCW;
  pos = rot(pos, rotation);
  int steps = 0;
  while (detect_obstacle(sensor)) {
    pos = move(pos, FORWARDS);
    steps++;
  }
  pos = rot(pos, opposite_rot);
  pos = move(pos, FORWARDS);
  while (detect_obstacle(sensor)) pos = move(pos, FORWARDS);
  pos = rot(pos, opposite_rot);
  while (steps) {
    pos = move(pos, FORWARDS);
    steps--;
  }
  pos = rot(pos, rotation);
}

Position avoid_border(Position pos) {
  Rotation rotation = pos.facing == POSX ? CCW : CW;
  Rotation opposite_rot = pos.facing == POSX ? CW : CCW;
  pos = rot(pos, rotation);
  pos = move(pos, FORWARDS);
  pos = rot(pos, opposite_rot);
  return pos;
}
void defuse_mine() {
  defuser.write(60);
  tone(BUZZER, 50);
  delay(500);
  noTone(BUZZER);
  delay(1500);
  defuser.write(0);
  Serial.println("Bomb defused");
}
bool detect_obstacle(Sensor sensor) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  int duration = pulseIn(sensor, HIGH);
  int distance = duration * 0.0343 / 2;
  if (distance < 100) return true;
  return false;
}
void detect_mine() {
  if (analogRead(PHOTO_RESISTOR) >= 500) {
    defuse_mine();
    Serial.println("Bomb detected");
  }
}
bool check_border(Position pos) {
  if ((pos.x == 0 || pos.x == 50) && pos.y == 60) {
    Serial.println("End of Map");
    delay(10000);
    return false;
  }
  if (pos.facing == POSX && pos.x == 50) return true;
  if (pos.facing == NEGX && pos.x == 0) return true;
  return false;
}

void setup() {
  pinMode(L1, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(PHOTO_RESISTOR, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(FRONT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);
  pinMode(LEFT_SENSOR, INPUT);

  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);
  digitalWrite(BUZZER, LOW);
  digitalWrite(TRIG, LOW);

  defuser.attach(SERVO);
  defuser.write(0);
  Serial.begin(9600);
}


void loop() {
  static Position pos{ 0, 0, POSX };
  pos = move(pos, FORWARDS);
  detect_mine();
  if (detect_obstacle(front_sensor)) pos = avoid_obstacle(pos);
  if (check_border(pos)) pos = avoid_border(pos);
}
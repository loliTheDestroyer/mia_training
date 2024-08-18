#define R1 4
#define R2 5
#define L1 2
#define L2 3
#define FRD_BTN 10
#define BCK_BTN 11
#define RTL_BTN 13
#define RTR_BTN 12

enum Facing { POSX = 0, NEGY, NEGX, POSY };
enum Direction { FORWARDS = 1, BACKWARDS = -1 };
enum Rotation { CW = 1, CCW = -1 };
struct Position { int x; int y; Facing facing; };

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
    return pos;
}

Position rot(const Position pos, const Rotation rot) {
    // POSX(0) <-> NEGY(1) <-> NEGX(2) <-> POSY(3) (mod 4) (+ 4 to handle neg numbers)
    const int facing = (pos.facing + rot + 4) % 4;

    if (rot == CW)
        move_motors(FORWARDS, BACKWARDS);
    else
        move_motors(BACKWARDS, FORWARDS);

    return {pos.x, pos.y, static_cast<Facing>(facing)};
}


void setup() {
    pinMode(L1, OUTPUT);
    pinMode(R1, OUTPUT);
    pinMode(L2, OUTPUT);
    pinMode(R2, OUTPUT);
    pinMode(FRD_BTN, INPUT_PULLUP);
    pinMode(BCK_BTN, INPUT_PULLUP);
    pinMode(RTL_BTN, INPUT_PULLUP);
    pinMode(RTR_BTN, INPUT_PULLUP);

    digitalWrite(L1, LOW);
    digitalWrite(L2, LOW);
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);

    Serial.begin(9600);
}
void print_pos(const Position pos) {
    Serial.print("(");
    Serial.print(pos.x);
    Serial.print(", ");
    Serial.print(pos.y);
    Serial.println(")");
}

void loop() {
    static Position pos {0, 0, POSX};

    if (digitalRead(FRD_BTN) == LOW) pos = move(pos, FORWARDS);
    else if (digitalRead(BCK_BTN) == LOW) pos = move(pos, BACKWARDS);
    else if (digitalRead(RTR_BTN) == LOW) pos = rot(pos, CW);
    else if (digitalRead(RTL_BTN) == LOW) pos = rot(pos, CCW);
    else return;

    print_pos(pos);
}
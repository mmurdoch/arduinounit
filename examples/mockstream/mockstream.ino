#line 2 "mockstream.ino"
#include <ArduinoUnit.h>

Stream *io = &Serial;

// safe way to save/restore stream
struct MockIO : MockStream {
  Stream *save;
  MockIO() { save = io; io=this; }
  ~MockIO() { io = save; }
};

int readX() {
  io->print("x? ");
  int x = io->parseInt();
  return x;
}

test(readX) {
  MockIO mock;
  int x = -30;
  mock.input.print(x);
  assertEqual(readX(),x);
  assertEqual(mock.output,"x? ");
}

int readY() {
  io->print("y? ");
  int y = io->parseInt();
  return y;
}

test(readY) {
  MockIO mock;
  int y = -30;
  mock.input.print(y);
  assertEqual(readY(),y);
  assertEqual(mock.output,"y? ");
}

void format(int x, int y, int sum) {
  io->print(x);
  io->print("+");
  io->print(y);
  io->print("=");
  io->print(sum);
  io->println(".");
}

test(format) {
  MockIO mock;
  format(3,5,8);
  assertEqual(mock.output,"3+5=8.\r\n");
}

void add() {
  int x = readX();
  int y = readY();
  int sum = x + y;
  format(x,y,sum);
}

test(add) {
  MockIO mock;
  mock.input.println(73);
  mock.input.println(22);
  assertEqual(mock.input,"73\r\n22\r\n");
  assertEqual(mock.output,"");
  add();
  assertEqual(mock.input,"\r\n");  
  assertEqual(mock.output,"x? y? 73+22=95.\r\n");
}

void setup() {
  Serial.begin(9600);
  while(!Serial) {} // Portability for Leonardo/Micro
}

void loop() {
  Test::run();
}

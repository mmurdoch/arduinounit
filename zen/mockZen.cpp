#if !defined(ARDUINO)

// only used for "en vitro" tests (not on actual board)

#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "ArduinoUnit.h"

int random(int n) {
  static bool setup = false;
  if (!setup) {
    srand(time(0));
    setup = true;
  }
  return rand() % n;
}

int random(int a, int b) {
  return a+rand() % (b-a+1);
}

void setup();
void loop();

CppIOStream Serial;

int main(int argc, char *argv[]) {
  setup();

  // parse --exclude/-e <pattern> and --include/-i <pattern> commands
  for (int i=1; i<argc; ++i) {
    if (strcmp(argv[i],"--exclude")==0 || strcmp(argv[i],"-e")==0) {
      ++i;
      Test::exclude(argv[i]);
      continue;
    }
    if (strcmp(argv[i],"--include")==0 || strcmp(argv[i],"-i")==0) {
      ++i;
      Test::include(argv[i]);
      continue;
    }
    if (strcmp(argv[i],"--")==0) { break; }
    std::cerr << "unknown argument '" << argv[i] << "'" << std::endl;
    exit(1);
  }

  // instead of looping forever, loop while there are active tests
  while (Test::remaining() > 0) {
    loop();
  }
  return 0;
}

#define INPUT 0
#define OUTPUT 1
#define HIGH 1
#define LOW 0

bool pins[100];
void pinMode(int pin, int mode) {}
bool digitalRead(int pin) { return pins[pin]; }
void digitalWrite(int pin, int value) { pins[pin] = (value != 0); }
int analogRead(int pin) { return random(0,1023); }



#include "zen.ino"

#endif

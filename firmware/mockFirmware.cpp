#if !defined(ARDUINO)

// only used for "en vitro" tests (not on actual board)

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <time.h>
#include <sys/time.h>
#include "ArduinoUnit.h"
#include "ArduinoUnitMock.h"

int random(int n) { return rand() % n; }
int random(int a, int b) { return a+rand() % (b-a+1); }

std::istringstream in("run\n");
CppIOStream Serial(in,std::cout);

void setup();
void loop();

int main(int argc, char *argv[]) {
  srand(time(0));
  
  setup();

  Test::out = &Serial;

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

  while (Test::remaining() > 0) {
    loop();
  }
  return 0;
}

#include "firmware.ino"

#endif

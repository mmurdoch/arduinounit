#if !defined(ARDUINO)

// only used for "en vitro" tests (not on actual board)

#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
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

unsigned long millis() {
  static bool setup = false;
  static struct timeval starttime;
  struct timeval now;
  if (!setup) {
    gettimeofday(&starttime, NULL);
    setup = true;
  }
  gettimeofday(&now, NULL);
  double secs = (double)(now.tv_usec - starttime.tv_usec) / 1000000.0
    + (double)(now.tv_sec - starttime.tv_sec);
  return secs*1000;
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

#include "advanced.ino"

#endif

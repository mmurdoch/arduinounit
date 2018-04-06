#if !defined(ARDUINO)

// only used for "en vitro" tests (not on actual board)

#include "ArduinoUnit.h"

CppIOStream Serial;

void setup();
void loop();

int main(int argc, char *argv[]) {
  setup();

  Test::out = &Serial;
  while (Test::remaining() > 0) {
    loop();
  }
  return 0;
}

#include "mockstream.ino"

#endif


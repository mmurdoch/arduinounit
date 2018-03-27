#if !defined(ARDUINO)

void setup();
void loop();

int main() {
  setup();
  for (;;) loop();
  return 0;
}

#define F(X) X
struct FakeSerial {
  void begin(long baud) { (void) baud; }
  bool operator!() const { return false; }
} Serial;

#endif

#include "verbosity.ino"


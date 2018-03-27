#if !defined(ARDUINO)

// only used for "en vitro" tests (not on actual board)

#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

struct timeval starttime;

void setup();
void loop();

int main() {
  gettimeofday(&starttime, NULL);
  srand(time(0));
  setup();
  for (;;) loop();
  return 0;
}

#define F(X) X
struct FakeSerial {
  void begin(long baud) { (void) baud; }
  bool operator!() const { return false; }
} Serial;


int random(int n) { return rand() % n; }
int random(int a, int b) { return a+rand() % (b-a+1); }

unsigned long millis() {
  struct timeval now;
  gettimeofday(&now, NULL);
  double secs = (double)(now.tv_usec - starttime.tv_usec) / 1000000 + (double)(now.tv_sec - starttime.tv_sec);
  return secs*1000;
}

#include "advanced.ino"

#endif

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <chrono>
#include <thread>
#include <math.h>
#include "ArduinoUnitUtility/ArduinoUnitMockTime.h"

double myabs(double x) { return x < 0 ? -x : x; }

void testTime() {
  assert(micros() < 1000000);
  assert(millis() < 1000);

  {
    unsigned long time = 250000;
    double start = micros();
    std::this_thread::sleep_for(std::chrono::microseconds(time));
    double end = micros();
    assert(myabs((end-start)-(double) time) < 10000);
  }

  {
    unsigned long time = 250;
    double long start = millis();
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
    double end = millis();
    assert(myabs((end-start)-(double) time) < 10);
  }
  
  {
    unsigned long time = 250000;
    double start = micros();
    delayMicroseconds(time);
    double end = micros();
    assert(myabs((end-start)-(double) time) < 10000);
  }
  
  {
    unsigned long time = 250;
    double long start = millis();
    delay(time);
    double end = millis();
    assert(myabs((end-start)-(double) time) < 10);
  }
}

int main() {
  testTime();
  printf("ok\n");
}

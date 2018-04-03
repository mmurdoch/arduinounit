#include "ArduinoUnitMockTime.h"

#if !defined(ARDUINO)

#include <thread>
#include <chrono>

struct ArduinoUnitMockTime {
  const std::chrono::high_resolution_clock::time_point start;
  
  ArduinoUnitMockTime() : start(std::chrono::high_resolution_clock::now()) {
  }
  
  unsigned long long micros() const {
    std::chrono::high_resolution_clock::time_point current
      = std::chrono::high_resolution_clock::now();
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(current-start).count();
    return microseconds;
  }

  unsigned long long millis() const {
    std::chrono::high_resolution_clock::time_point current
      = std::chrono::high_resolution_clock::now();
    long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(current-start).count();
    return milliseconds;
  }
};

static ArduinoUnitMockTime arduinoUnitMockTime;

unsigned long micros() { return arduinoUnitMockTime.micros(); }
unsigned long millis() { return arduinoUnitMockTime.millis(); }
void delay(unsigned int duration) {
    std::this_thread::sleep_for(std::chrono::milliseconds(duration));
}
void delayMicroseconds(unsigned int duration) {
    std::this_thread::sleep_for(std::chrono::microseconds(duration));
}

#endif


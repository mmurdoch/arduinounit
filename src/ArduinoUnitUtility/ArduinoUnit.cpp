#if defined(ARDUINO)
#include <Arduino.h>
#endif

#include "../ArduinoUnit.h"

const uint8_t Test::UNSETUP = 0;
const uint8_t Test::LOOPING = 1;
const uint8_t Test::DONE_SKIP = 2;
const uint8_t Test::DONE_PASS = 3;
const uint8_t Test::DONE_FAIL = 4;


Test* Test::root = 0;
Test* Test::current = 0;

uint16_t Test::count = 0;
uint16_t Test::passed = 0;
uint16_t Test::failed = 0;
uint16_t Test::skipped = 0;
uint8_t Test::max_verbosity = TEST_VERBOSITY_ALL;
uint8_t Test::min_verbosity = TEST_VERBOSITY_TESTS_SUMMARY;

#if defined(ARDUINO)
Print* Test::out = &Serial;
#else
std::ostream * Test::out = &std::cout;
#endif

void Test::noMessage(bool ok) { (void) ok; }

void Test::resolve() 
{
  bool pass = current->state==DONE_PASS;
  bool fail = current->state==DONE_FAIL;
  bool skip = current->state==DONE_SKIP;
  bool done = (pass || fail || skip);
  
  if (done) {
    if (pass) ++Test::passed;
    if (fail) ++Test::failed;
    if (skip) ++Test::skipped;
    
#if TEST_VERBOSITY_EXISTS(TESTS_SKIPPED) || TEST_VERBOSITY_EXISTS(TESTS_PASSED) || TEST_VERBOSITY_EXISTS(TESTS_FAILED)
    
    bool output = false;

    output = output || (skip && TEST_VERBOSITY(TESTS_SKIPPED));
    output = output || (pass && TEST_VERBOSITY(TESTS_PASSED));
    output = output || (fail && TEST_VERBOSITY(TESTS_FAILED));

    if (output) {
      ArduinoUnitPrint(ARDUINO_UNIT_STRING("Test "));
      ArduinoUnitPrint(name);
#if TEST_VERBOSITY_EXISTS(TESTS_SKIPPED)
      if (skip) { ArduinoUnitPrintln(ARDUINO_UNIT_STRING(" skipped.")); }
#endif
      
#if TEST_VERBOSITY_EXISTS(TESTS_PASSED)
      if (pass) { ArduinoUnitPrintln(ARDUINO_UNIT_STRING(" passed.")); }
#endif
      
#if TEST_VERBOSITY_EXISTS(TESTS_FAILED)
      if (fail) { ArduinoUnitPrintln(ARDUINO_UNIT_STRING(" failed.")); }
#endif
    }
#endif
  }
#if TEST_VERBOSITY_EXISTS(TESTS_SUMMARY)
  if (root == 0 && TEST_VERBOSITY(TESTS_SUMMARY)) {
    ArduinoUnitPrint(ARDUINO_UNIT_STRING("Test summary: "));
    ArduinoUnitPrint(passed);
    ArduinoUnitPrint(ARDUINO_UNIT_STRING(" passed, "));
    ArduinoUnitPrint(failed);
    ArduinoUnitPrint(ARDUINO_UNIT_STRING(" failed, and "));
    ArduinoUnitPrint(skipped);
    ArduinoUnitPrint(ARDUINO_UNIT_STRING(" skipped, out of "));
    ArduinoUnitPrint(count);
    ArduinoUnitPrintln(ARDUINO_UNIT_STRING(" test(s)."));
  }
#endif
}

void Test::remove()
{
  for (Test **p = &root; *p != 0; p=&((*p)->next)) {
    if (*p == this) {
      *p = (*p)->next;
      break;
    }
  }
}

#if ARDUINO_UNIT_USE_FLASH > 0
Test::Test(const __FlashStringHelper *_name, uint8_t _verbosity)
  : name(_name), verbosity(_verbosity)
{
  insert();
}
#endif

Test::Test(const char *_name, uint8_t _verbosity)
  : name(_name), verbosity(_verbosity)
{
  insert();
}

void Test::insert()
{
  state = UNSETUP;
  { // keep list sorted
    Test **p = &Test::root;
    while ((*p) != 0) {
      if (name.compare((*p)->name) <= 0) break;
      p=&((*p)->next);
    }
    next=(*p);
    (*p)=this;
  }
  ++Test::count;
}

void Test::pass() { state = DONE_PASS; }
void Test::fail() { state = DONE_FAIL; }
void Test::skip() { state = DONE_SKIP; }

void Test::setup() {};

void Test::run()
{
  for (Test **p = &root; (*p) != 0; ) {
    current = *p;

    if (current->state == LOOPING) {
      current->loop();
    } else if (current->state == UNSETUP) {
      current->setup();
      if (current->state == UNSETUP) {
        current->state = LOOPING;
      }
    }

    if (current->state != LOOPING) {
      (*p)=((*p)->next);
      current->resolve();
    } else {
      p=&((*p)->next);
    }

#if defined(ESP8266) || defined(ESP32)
    yield();
#endif
  }
}

Test::~Test()
{
  remove();
}

void Test::include(const char *pattern)
{
  for (Test *p = root; p != 0; p=p->next) {
    if (p->state == DONE_SKIP && p->name.matches(pattern)) {
      p->state = UNSETUP;
    }
  }
}

void Test::exclude(const char *pattern)
{
  for (Test *p = root; p != 0; p=p->next) {
    if (p->state == UNSETUP && p->name.matches(pattern)) {
      p->state = DONE_SKIP;
    }
  }
}

#if ARDUINO_UNIT_USE_FLASH > 0
TestOnce::TestOnce(const __FlashStringHelper *name) : Test(name) {}
#endif

TestOnce::TestOnce(const char *name) : Test(name) {}

void TestOnce::loop() 
{
  once();
  if (state == LOOPING) state = DONE_PASS;
}

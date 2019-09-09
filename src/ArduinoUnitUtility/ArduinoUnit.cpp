#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <iostream>
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

#if !defined(ARDUINO)
static CppStreamPrint Serial;
#endif

Print* Test::out = &Serial;

#if ARDUINO_UNIT_USE_FLASH > 0
#define ARDUINO_UNIT_PROGMEM PROGMEM
#else
#define ARDUINO_UNIT_PROGMEM
#endif

const char CONST_SKIPPED[] ARDUINO_UNIT_PROGMEM = "skipped";
const char CONST_PASSED[]  ARDUINO_UNIT_PROGMEM = "passed";
const char CONST_FAILED[]  ARDUINO_UNIT_PROGMEM = "failed";

ARDUINO_UNIT_DECLARE_STRING SKIPPED = (ARDUINO_UNIT_DECLARE_STRING) CONST_SKIPPED;
ARDUINO_UNIT_DECLARE_STRING PASSED = (ARDUINO_UNIT_DECLARE_STRING) CONST_PASSED;
ARDUINO_UNIT_DECLARE_STRING FAILED = (ARDUINO_UNIT_DECLARE_STRING) CONST_FAILED;

void Test::noMessage(bool ok) { (void) ok; }

void Test::resolve() 
{
  bool pass = state==DONE_PASS;
  bool fail = state==DONE_FAIL;
  bool skip = state==DONE_SKIP;
  bool done = (pass || fail || skip);
  ARDUINO_UNIT_DECLARE_STRING  message = 0;
  
  if (done) {
    if (pass) { message=PASSED; ++Test::passed; }
    if (fail) { message=FAILED; ++Test::failed; }
    if (skip) { message=SKIPPED; ++Test::skipped; }
    
#if TEST_VERBOSITY_EXISTS(TESTS_SKIPPED) || TEST_VERBOSITY_EXISTS(TESTS_PASSED) || TEST_VERBOSITY_EXISTS(TESTS_FAILED)
    
    bool output = false;

    output = output || (skip && TEST_VERBOSITY(TESTS_SKIPPED));
    output = output || (pass && TEST_VERBOSITY(TESTS_PASSED));
    output = output || (fail && TEST_VERBOSITY(TESTS_FAILED));

    if (output) {
      Test::out->print(ARDUINO_UNIT_STRING("Test "));
      Test::out->print(name);
      Test::out->print(' ');  
      Test::out->print(message);
      Test::out->println();
    }
#endif
  }
#if TEST_VERBOSITY_EXISTS(TESTS_SUMMARY)
  if (root == 0 && TEST_VERBOSITY(TESTS_SUMMARY)) {
    Test::out->print(ARDUINO_UNIT_STRING("Test summary: "));
    Test::out->print(passed);
    Test::out->print(' ');
    Test::out->print(PASSED);
    Test::out->print(ARDUINO_UNIT_STRING(", "));
    Test::out->print(failed);
    Test::out->print(' ');
    Test::out->print(FAILED);
    Test::out->print(ARDUINO_UNIT_STRING(", "));    
    Test::out->print(skipped);
    Test::out->print(' ');
    Test::out->print(SKIPPED);
    Test::out->print(ARDUINO_UNIT_STRING(", out of "));
    Test::out->print(count);
    Test::out->println(ARDUINO_UNIT_STRING(" test(s)."));
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

Test::Test(const __FlashStringHelper *_name, uint8_t _verbosity)
  : name(_name), verbosity(_verbosity)
{
  insert();
}

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
      if (name.compareTo((*p)->name) <= 0) break;
      p=&((*p)->next);
    }
    next=(*p);
    (*p)=this;
  }
  ++Test::count;
}

void Test::pass() { if (current != 0) current->state = DONE_PASS; }
void Test::fail() { if (current != 0) current->state = DONE_FAIL; }
void Test::skip() { if (current != 0) current->state = DONE_SKIP; }

void Test::setup() {};

int Test::remaining() {
  return count - (skipped + passed + failed);
}

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
  current = 0;
}

void Test::abort()
{
  while (root != 0) {
    current=root;
    current->state = DONE_FAIL;
    Test::out->print(ARDUINO_UNIT_STRING("Assertion failed: test "));
    Test::out->print(current->name);
    Test::out->print(ARDUINO_UNIT_STRING(" aborted, file ArduinoUnit.cpp, line "));
    Test::out->print(__LINE__);
    Test::out->println(ARDUINO_UNIT_STRING("."));
    root=root->next;
    current->resolve();
  }
  current=0;
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

TestOnce::TestOnce(const __FlashStringHelper *name) : Test(name) {}

TestOnce::TestOnce(const char *name) : Test(name) {}

void TestOnce::loop() 
{
  once();
  if (state == LOOPING) state = DONE_PASS;
}

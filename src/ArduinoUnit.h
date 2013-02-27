#pragma once

#include <stdint.h>
#include <Print.h>

#define ARDUINO_UNIT_MAJOR_VERSION 2
#define ARDUINO_UNIT_MINOR_VERSION 0

//
// These define what you want for output from tests.
//
#define  TEST_VERBOSITY_TESTS_SUMMARY     0x01
#define  TEST_VERBOSITY_TESTS_FAILED      0x02
#define  TEST_VERBOSITY_TESTS_PASSED      0x04
#define  TEST_VERBOSITY_TESTS_SKIPPED     0x08
#define  TEST_VERBOSITY_TESTS_ALL         0x0F
#define  TEST_VERBOSITY_ASSERTIONS_FAILED 0x10
#define  TEST_VERBOSITY_ASSERTIONS_PASSED 0x20
#define  TEST_VERBOSITY_ASSERTIONS_ALL    0x30
#define  TEST_VERBOSITY_ALL               0x3F
#define  TEST_VERBOSITY_NONE              0x00

//
// maximum verbosity available (independent of verbosity field)
// change this only to save code space
//
#ifndef TEST_MAX_VERBOSITY
#define TEST_MAX_VERBOSITY TEST_VERBOSITY_ALL
#endif

#define TEST_VERBOSITY_EXISTS(OF) ((TEST_MAX_VERBOSITY & TEST_VERBOSITY_ ## OF) != 0)

#define TEST_VERBOSITY_ALLOWED(OF) (TEST_VERBOSITY_EXISTS(OF) && ((Test::max_verbosity & TEST_VERBOSITY_ ## OF) != 0))

#define TEST_VERBOSITY_REQUIRED(OF) (TEST_VERBOSITY_ALLOWED(OF) && ((Test::min_verbosity & TEST_VERBOSITY_ ## OF) != 0))

#define TEST_VERBOSITY(OF) (TEST_VERBOSITY_ALLOWED(OF) && (((Test::min_verbosity & TEST_VERBOSITY_ ## OF ) != 0) || (((Test::current != 0) && ((Test::current->verbosity & TEST_VERBOSITY_ ## OF) != 0)))))

//
// Core unit test class.
//
// There are two convenience macros for extending this class and
// implement a one-step test (test macro), or a multi-step test
// (testing macro).
//
//  // One shot (first loop()) test
//
//  test(me_once) 
//  {
//    int x=1,y=2;
//    assertNotEqual(x,y);
//  }
//
//
//  // Continuous (every loop() until pass(), fail(), or skip()) test
//
//  testing(me_often) {
//    assertEqual(digitalRead(errorPin),LOW);
//  }
//
//  //
//  // Roll-your-own test:
//  //
//
//  class MyTest : public Test {
//  public:
//    MyTest(const char *name) : Test(name) {} { /* must name test */ }
//    void setup() { /* can skip(), pass(), or fail() here */ }
//    void loop()  { /* eventually calls pass() or fail() */ }
//  };
//
//  void setup() {
//    // all tests are included by default
//    Test::exclude("*_skip");
//    if (no_slow_tests) Test::exclude("*_slow");
//    if (all_crypto_tests) {
//      Test::include("crypto_*");
//      Test::exclude("crypto_*_skip");
//    }
//    Test::setup_all();
//  }
//
//  void loop() {
//    Test::loop_all();
//  }
//
//  Variables you might want to adjust:
//
//    static Print* Test::out -- the stream that is used for output
//      -- defaults to
//
//         &Serial
//
//      -- This affects the output of all tests
//      
//    uint8_t verbosity -- how much to report on output
//
//      -- defaults to 
//
//         TEST_VERBOSITY_ASSERTIONS_FAILED|TEST_VERBOSITY_TESTS_ALL
//
//      -- to keep code small, reporting code that is not set in 
//      
//         TEST_MAX_VERBOSITY
//
//         is removed, so setting verbosity bits outside this mask
//         has no effect.  The default mask is to have all output
//         available, and the only reason to change this is to save
//         some code space.
//

class Test
{
 public:

  // allows for both ram/progmem based names
  class String : public Printable {
  public:
    const uint32_t data;
    String(const char *_data);
    String(const __FlashStringHelper *_data);
    void read(void *destination, uint16_t offset, uint8_t length) const;
    uint16_t length() const;
    int8_t compare(const Test::String &to) const;
    size_t printTo(Print &p) const;
    bool matches(const char *pattern) const;
  };

 private:
  // linked list structure for active tests
  static Test* root;
  Test *next;

  // static statistics for tests
  static uint16_t passed;
  static uint16_t failed;
  static uint16_t skipped;
  static uint16_t count;

  void resolve();
  void remove();
  void insert();

 public:  
  static uint8_t max_verbosity;
  static uint8_t min_verbosity;

  static inline uint16_t getCurrentPassed() { return passed; }
  static inline uint16_t getCurrentSkipped() { return skipped; }
  static inline uint16_t getCurrentFailed() { return failed; }
  static uint16_t getCurrentCount() { return count; }

  static const uint8_t UNSETUP;
  static const uint8_t LOOPING;
  static const uint8_t DONE_SKIP;
  static const uint8_t DONE_PASS;
  static const uint8_t DONE_FAIL;

  // output stream for all tests
  static Print *out;

  // UNSETUP, LOOPING, DONE_PASS, DONE_FAIL, DONE_SKIP
  uint8_t state;

  // current active test (=0 if none are active)
  static Test* current;

  // defaults to TEST_VERBOSITY_TESTS_ALL|TEST_VERBOSITY_ASSERTS_FAILED
  uint8_t verbosity;

  // the name of this test
  String name;

  void pass();
  void fail();
  void skip(); 

  virtual void setup();
  virtual void loop() = 0;

  // include (use) currently excluded (skipped) tests that match some
  // wildcard (*) pattern like,
  //
  //    "dev_*", "my_main_test", "*_quick"
  //
  // Since all tests are included by default, this is not useful except
  // after an exclude() call.
  // 
  // This should be done inside your setup() function.
  //
  static void include(const char *pattern);

  // exclude (skip) currently included tests that match some
  // wildcard (*) pattern like,
  //
  //    "my_broken_test", "*_skip", "*", "io_*", etc.  
  //
  // This should be done insize your setup() function.
  static void exclude(const char *pattern);

  //
  // Setup all configured tests:
  //
  // Simple usage:
  //
  // void setup() {
  //   Serial.begin(9600);
  // }
  //
  // void loop() {
  //   Test::run();
  // }
  //
  // Complex usage:
  //
  // void setup() {
  //   Test::exclude("*"); // exclude everything
  //   Test::include("io_*"); // but include io_* tests
  //   Test::exclude("io_*_lcd"); // except io_*_lcd tests
  //   Test::include("crypto_*_aes128"); // and use all crypto_*_aes128 tests
  // }
  //
  // void loop() {
  //   Test::run();
  // }
  //
  static void run();

  // Construct a test with a given name and verbosity level
  Test(const __FlashStringHelper *_name, uint8_t _verbosity = TEST_VERBOSITY_TESTS_ALL|TEST_VERBOSITY_ASSERTIONS_FAILED);

  Test(const char *_name, uint8_t _verbosity = TEST_VERBOSITY_TESTS_ALL|TEST_VERBOSITY_ASSERTIONS_FAILED);

  virtual ~Test();

  template <typename T>
    static bool assertion(const __FlashStringHelper *file, uint16_t line, const __FlashStringHelper *lhss, const T& lhs, const __FlashStringHelper *ops, bool (*op)(const T& lhs, const T& rhs), const __FlashStringHelper *rhss, const T& rhs) {
    bool ok = op(lhs,rhs);
    bool output = false;

    if ((!ok) && (current != 0)) current->fail();

#if TEST_VERBOSITY_EXISTS(ASSERTIONS_PASSED)
    if (ok && TEST_VERBOSITY(ASSERTIONS_PASSED)) {
      output = true;
    }
#endif

#if TEST_VERBOSITY_EXISTS(ASSERTIONS_FAILED)
    if ((!ok) && TEST_VERBOSITY(ASSERTIONS_FAILED)) {
      output = true;
    }
#endif

#if TEST_VERBOSITY_EXISTS(ASSERTIONS_FAILED) || TEST_VERBOSITY_EXISTS(ASSERTIONS_PASSED)
    if (output) {
      out->print(file);
      out->print(F(":"));
      out->print(line);
      out->print(F(":1: "));
      out->print(ok ? F("pass") : F("fail"));
      out->print(F(" assert ("));
      out->print(lhss);
      out->print(F("="));
      out->print(lhs);
      out->print(F(") "));
      out->print(ops);
      out->print(F(" ("));
      out->print(rhss);
      out->print(F("="));
      out->print(rhs);
      out->print(F(")"));
      out->println();
    }
#endif
    return ok;
  }
};

class TestOnce : public Test {
 public:
  TestOnce(const __FlashStringHelper *name);
  TestOnce(const char *name);
  void loop();
  virtual void once() = 0;
};

// Template binary operators to assist with assertions

template <typename T>
bool isEqual(const T& a, const T& b) { return a==b; }


template <typename T>
bool isNotEqual(const T& a, const T& b) { return !(a==b); }

template <typename T>
bool isLess(const T& a, const T& b) { return a < b; }

template <typename T>
bool isMore(const T& a, const T& b) { return b < a; }

template <typename T>
bool isLessOrEqual(const T& a, const T& b) { return !(b<a); }

template <typename T>
bool isMoreOrEqual(const T& a, const T& b) { return !(a<b); }

template <> bool isLess<const char*>(const char* const &a, const char* const &b);
template <> bool isLessOrEqual<const char*>(const char* const &a, const char* const &b);
template <> bool isEqual<const char*>(const char* const &a, const char* const &b);
template <> bool isNotEqual<const char*>(const char* const &a, const char* const &b);
template <> bool isMore<const char*>(const char* const &a, const char* const &b);
template <> bool isMoreOrEqual<const char*>(const char* const &a, const char* const &b);


// simple one-shot test, usually checked with assertXXXX
#define test(name) struct test_ ## name : TestOnce { test_ ## name() : TestOnce(F(#name)) {}; void once(); } test_ ## name ## _instance; void test_ ## name :: once() 

// Note that a test-once test is defined in another source file.
//
// This is only necessary if you use assertTestXXXX when the test
// is in another file (or defined after the assertion on it).
#define externTest(name) struct test_ ## name : TestOnce { test_ ## name(); void once(); }; extern test_##name test_##name##_instance

// continuous test, probably with assertXXXX and an eventual pass() or fail()
#define testing(name) struct test_ ## name : Test { test_ ## name() : Test(F(#name)) {}; void loop(); } test_ ## name ## _instance; void test_ ## name :: loop() 

// Note that a test-until-skip-pass-or-fail test is defined in another
// source file.
//
// This is only necessary if you use assertTestXXXX when the test
// is in another file (or defined after the assertion on it).
#define externTesting(name) struct test_ ## name : Test { test_ ## name(); void loop(); }; extern test_##name test_##name##_instance

// helper define for the operators below
#define assertOp(arg1,op,op_name,arg2) if (!Test::assertion<typeof(arg2)>(F(__FILE__),__LINE__,F(#arg1),(arg1),F(op_name),op,F(#arg2),(arg2))) return;

// basic asserts
#define assertEqual(arg1,arg2)       assertOp(arg1,isEqual,"==",arg2)
#define assertNotEqual(arg1,arg2)    assertOp(arg1,isNotEqual,"!=",arg2)
#define assertLess(arg1,arg2)        assertOp(arg1,isLess,"<",arg2)
#define assertMore(arg1,arg2)        assertOp(arg1,isMore,">",arg2)
#define assertLessOrEqual(arg1,arg2) assertOp(arg1,isLessOrEqual,"<=",arg2)
#define assertMoreOrEqual(arg1,arg2) assertOp(arg1,isMoreOrEqual,">=",arg2)

#define assertTrue(arg) assertEqual(arg,true)
#define assertFalse(arg) assertEqual(arg,false)

#define checkTestDone(name) (test_##name##_instance.state >= Test::DONE_SKIP)
#define checkTestNotDone(name) (test_##name##_instance.state < Test::DONE_SKIP)

#define checkTestPass(name) (test_##name##_instance.state == Test::DONE_PASS)
#define checkTestNotPass(name) (test_##name##_instance.state != Test::DONE_PASS)

#define checkTestFail(name) (test_##name##_instance.state == Test::DONE_FAIL)
#define checkTestNotFail(name) (test_##name##_instance.state != Test::DONE_FAIL)

#define checkTestSkip(name) (test_##name##_instance.state == Test::DONE_SKIP)
#define checkTestNotSkip(name) (test_##name##_instance.state != Test::DONE_SKIP)

#define assertTestDone(name) assertMoreOrEqual(test_##name##_instance.state,Test::DONE_SKIP)
#define assertTestNotDone(name) assertLess(test_##name##_instance.state,Test::DONE_SKIP)

#define assertTestPass(name) assertEqual(test_##name##_instance.state,Test::DONE_PASS)
#define assertTestNotPass(name) assertNotEqual(test_##name##_instance.state,Test::DONE_PASS)
#define assertTestFail(name) assertEqual(test_##name##_instance.state,Test::DONE_FAIL)
#define assertTestNotFail(name) assertNotEqual(test_##name##_instance.state,Test::DONE_FAIL)
#define assertTestSkip(name) assertEqual(test_##name##_instance.state,Test::DONE_SKIP)
#define assertTestNotSkip(name) assertNotEqual(test_##name##_instance.state,Test::DONE_SKIP)

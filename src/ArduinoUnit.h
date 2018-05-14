#pragma once

/**

  @file ArduinoUnit.h

*/
#include <math.h>
#include "ArduinoUnitMock.h"

#include "ArduinoUnitUtility/Flash.h"
#include "ArduinoUnitUtility/Compare.h"
#include "ArduinoUnitUtility/ArduinoUnitWiden.h"
#include "ArduinoUnitUtility/ArduinoUnitString.h"

#include "ArduinoUnitUtility/FakeStream.h"
#include "ArduinoUnitUtility/FakeStreamBuffer.h"
#include "ArduinoUnitUtility/FreeMemory.h"

#define ArduinoUnitMacroArgs0(null,f,...) f
#define ArduinoUnitMacroArgs1(null,a1,f,...) f
#define ArduinoUnitMacroArgs2(null,a1,a2,f,...) f
#define ArduinoUnitMacroArgs3(null,a1,a2,a3,f,...) f
#define ArduinoUnitMacroArgs4(null,a1,a2,a3,a4,f,...) f
#define ArduinoUnitMacroArgs5(null,a1,a2,a3,a4,a5,f,...) f
#define ArduinoUnitMacroArgs6(null,a1,a2,a3,a4,a5,a6,f,...) f

#define ArduinoUnitMacroRecompose0(args) ArduinoUnitMacroArgs0 args
#define ArduinoUnitMacroRecompose1(args) ArduinoUnitMacroArgs1 args
#define ArduinoUnitMacroRecompose2(args) ArduinoUnitMacroArgs2 args
#define ArduinoUnitMacroRecompose3(args) ArduinoUnitMacroArgs3 args
#define ArduinoUnitMacroRecompose4(args) ArduinoUnitMacroArgs4 args
#define ArduinoUnitMacroRecompose5(args) ArduinoUnitMacroArgs5 args
#define ArduinoUnitMacroRecompose6(args) ArduinoUnitMacroArgs6 args

#define ArduinoUnitMacroChoose0(f,...) ArduinoUnitMacroRecompose0((null,##__VA_ARGS__,f##0))
#define ArduinoUnitMacroChoose1(f,...) ArduinoUnitMacroRecompose1((null,##__VA_ARGS__,f##1,f##0))
#define ArduinoUnitMacroChoose2(f,...) ArduinoUnitMacroRecompose2((null,##__VA_ARGS__,f##2,f##1,f##0))
#define ArduinoUnitMacroChoose3(f,...) ArduinoUnitMacroRecompose3((null,##__VA_ARGS__,f##3,f##2,f##1,f##0))
#define ArduinoUnitMacroChoose4(f,...) ArduinoUnitMacroRecompose4((null,##__VA_ARGS__,f##4,f##3,f##2,f##1,f##0))
#define ArduinoUnitMacroChoose5(f,...) ArduinoUnitMacroRecompose5((null,##__VA_ARGS__,f##5,f##4,f##3,f##2,f##1,f##0))
#define ArduinoUnitMacroChoose6(f,...) ArduinoUnitMacroRecompose6((null,##__VA_ARGS__,f##6,f##5,f##4,f##3,f##2,f##1,f##0))

/** \brief This is defined to manage the API transition to 2.X */
#define ARDUINO_UNIT_MAJOR_VERSION 2

/** \brief This is defined to manage the API transition to 2.X */
#define ARDUINO_UNIT_MINOR_VERSION 3

//
// These define what you want for output from tests.
//

/** 
\brief Verbosity mask for overall summary (default).

Verbosity mask for a 1-line summary of the form:

    test summary: P passed, F failed, and S skipped out of T test(s).

This summary happens once all the tests have been resolved (pass, fail or skip).
*/
#define  TEST_VERBOSITY_TESTS_SUMMARY     0x01

/**
\brief Verbosity mask for failed test summary (default).

Verbosity mask for a 1-line summary of a failed test of the form:

    test NAME failed.

This summary happens once the given test resolves (pass, fail, or skip).
*/
#define  TEST_VERBOSITY_TESTS_FAILED      0x02

/**
\brief Verbosity mask for passed test summary (default).

Verbosity mask for a 1-line summary of a passed test of the form:

    test NAME passed.

This summary happens once the given test resolves (pass, fail, or skip).
*/
#define  TEST_VERBOSITY_TESTS_PASSED      0x04


/**
\brief Verbosity mask for skipped test summary (default).

Verbosity mask for a 1-line summary of a skipped test of the form:

    test NAME skipped.

This summary happens once the given test resolves (pass, fail, or skip).
*/
#define  TEST_VERBOSITY_TESTS_SKIPPED     0x08


/**
\brief Verbosity mask for resolved (skip, pass, or fail) test summary (default).

Verbosity mask for a 1-line summary of a resolved test of the form:

    test NAME (passed|failed|skipped).

This summary happens once the given test resolves (pass, fail, or skip).
*/
#define  TEST_VERBOSITY_TESTS_ALL         0x0F

/**
\brief Verbosity mask for failed assertions (default).

Verbosity mask for a 1-line summary of failed assertions of the form:

    FILE:LINE:1 fail assert (NAME1=VALUE1) OP (NAME2=VALUE2)

*/
#define  TEST_VERBOSITY_ASSERTIONS_FAILED 0x10

/**
\brief Verbosity mask for passed assertions (default off).

Verbosity mask for a 1-line summary of passed assertions of the form:

    FILE:LINE:1 pass assert (NAME1=VALUE1) OP (NAME2=VALUE2)

*/
#define  TEST_VERBOSITY_ASSERTIONS_PASSED 0x20

/**
\brief Verbosity mask for all assertions (default fail).

Verbosity mask for a 1-line summary of failed assertions of the form:

    FILE:LINE:1 (pass|fail) assert (NAME1=VALUE1) OP (NAME2=VALUE2)

*/
#define  TEST_VERBOSITY_ASSERTIONS_ALL    0x30

/**
This is the default value for TEST_MAX_VERBOSITY, and Test::max_verbosity is this, so no output is globally suppressed.
*/
#define  TEST_VERBOSITY_ALL               0x3F

/**
Verbosity mask for no verbostiy.  The default value of Test::min_verbosity asks that failed assertions and test summaries be generated (TEST_VERBOSITY_TESTS_ALL|TEST_VERBOSITY_ASSERTIONS_FAILED).
*/
#define  TEST_VERBOSITY_NONE              0x00


#ifndef TEST_MAX_VERBOSITY
/**
Define what output code is included in the in the library (default TEST_VERBOSITY_ALL).

Clearing a mask in TEST_MAX_VERBOSITY eliminates the code related to that kind of output.  Change this only to save PROGMEM space.
*/
#define TEST_MAX_VERBOSITY TEST_VERBOSITY_ALL
#endif


/** \brief Check if given verbosity exists. (advanced)

    This is used to mask out code that would never be
    executed due to a cleared flag in TEST_MAX_VERBOSITY.
    It is used in, for example:

        #if TEST_VERBOSITY_EXISTS(TESTS_SKIPPED)
           maybe output something
        #endif

    This would only rarely be useful in custom assertions.
*/
#define TEST_VERBOSITY_EXISTS(OF) ((TEST_MAX_VERBOSITY & TEST_VERBOSITY_ ## OF) != 0)

/** \brief Check if given verbosity exists. (advanced)

    This is used to mask out code that would never be
    executed due to a cleared flag in TEST_MAX_VERBOSITY.
    It is used in, for example:

        #if TEST_VERBOSITY_EXISTS(TESTS_SKIPPED)
           maybe output something
        #endif

    This would only rarely be useful in custom assertions.
*/

/** \brief Check if given verbosity is allowed. (advanced)

    This would only rarely be useful in custom assertions.

*/
#define TEST_VERBOSITY_ALLOWED(OF) (TEST_VERBOSITY_EXISTS(OF) && ((Test::max_verbosity & TEST_VERBOSITY_ ## OF) != 0))

/** \brief Check if given verbosity is required. (advanced)

    This would only rarely be useful in custom assertions.

*/
#define TEST_VERBOSITY_REQUIRED(OF) (TEST_VERBOSITY_ALLOWED(OF) && ((Test::min_verbosity & TEST_VERBOSITY_ ## OF) != 0))

/** \brief Check if given verbosity is appropriate. (advanced)

    This would only rarely be useful in custom assertions.

*/
#define TEST_VERBOSITY(OF) (TEST_VERBOSITY_ALLOWED(OF) && (((Test::min_verbosity & TEST_VERBOSITY_ ## OF ) != 0) || (((Test::current != 0) && ((Test::current->verbosity & TEST_VERBOSITY_ ## OF) != 0)))))

/** 
There are two convenience macros for extending this class and
implement a one-step test (test macro), or a multi-step test
(testing macro).

One shot (first loop()) test
    
    test(me_once) 
    {
      int x=1,y=2;
      assertNotEqual(x,y);
    }
  
  
Continuous (every loop() until pass(), fail(), or skip()) test
  
    testing(me_often) {
      assertEqual(digitalRead(errorPin),LOW);
    }
  
Roll-your-own test:

    class MyTest : public Test {
    public:
      MyTest(const char *name) : Test(name) {} { 
        // can set verbosity here.
      }
      void setup() { 
        // can set verbosity here.
        // call call pass(), fail(), or skip()
        // can make assertions
      }
      void loop()  { 
        // can set verbosity here.
        // call call pass(), fail(), or skip()
        // can make assertions
      }
    };
  
    void setup() {
      // all tests are included by default
      Test::exclude("*_skip");
      if (no_slow_tests) Test::exclude("*_slow");
      if (all_crypto_tests) {
        Test::include("crypto_*");
        Test::exclude("crypto_*_skip");
      }
    }
  
    void loop() {
      Test::run();
    }

Variables you might want to adjust:

    static Print* Test::out

 - the stream that is used for output
 - defaults to

       &Serial

 - This affects the output of all tests
    
    uint8_t verbosity

 - how much to report on output

 -- defaults to 

       TEST_VERBOSITY_ASSERTIONS_FAILED|TEST_VERBOSITY_TESTS_ALL

 -- to keep code small, reporting code that is not set in 
    
       TEST_MAX_VERBOSITY

    is removed, so setting verbosity bits outside this mask
    has no effect.  The default mask is to have all output
    available, and the only reason to change this is to save
    some code space.
*/
class Test
{
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

  struct Printer {
    template <typename T> inline Printer &operator<<(const T &x) {
      Test::out->print((const typename ArduinoUnitWiden<T>::type &)x);
      return *this;   
    }
  };

  /** After the compile-time-mask TEST_MAX_VERBOSITY, this is a global
      run-time-mask of what output should be generated.
  */
  static uint8_t max_verbosity;

  /** After the compile-time-mask TEST_MAX_VERBOSITY, and the global
      (static) run-time-mask Test::max_verbosity of what output can be
      generated, this is a global (static) run-time-mask of what output
      should be generated. */
  static uint8_t min_verbosity;

  static inline uint16_t getCurrentPassed() { return passed; }
  static inline uint16_t getCurrentSkipped() { return skipped; }
  static inline uint16_t getCurrentFailed() { return failed; }
  static uint16_t getCurrentCount() { return count; }

  /** State of a test before a setup() call.  The exclude()
      function may move a test directly from UNSETUP to DONE_SKIP. */
  static const uint8_t UNSETUP;

  /** State of a test while actively in loop().  Tests are resolved
      by changing state one of the DONE_ states. */
  static const uint8_t LOOPING;

  /** State of a test that will be counted as skipped.  This can be
      done any time before resolving the test some other way, but
      should mean some small amount of steps to determine that no
      actual testing was done. */
  static const uint8_t DONE_SKIP;

  /** State of a passed test. */
  static const uint8_t DONE_PASS;

  /** State of a failed test. */
  static const uint8_t DONE_FAIL;

  /** /brief Output stream for all tests.  
      The default value of this is
      ```
          Test::out = &Serial;
      ```
      This places the output on the main serial port.  The library
      does not set the baud rate, so you must do so in your setup().

      To redirect all output to some other stream, say the Serial3 
      device of the arduino mega, use

          Serial3.begin(19200L);
          Test::out = &Serial3;

      in your setup().
  */
  static Print *out;

  /** The current state of this test.  It is one of:

           UNSETUP, LOOPING, DONE_PASS, DONE_FAIL, DONE_SKIP

  */
  uint8_t state;

  /** The current active test (=0 if none are active).  Asserts
      are allowed outside of tests, but just return if fail and
      potentially (according to min_verbosity and max_verbosity) 
      print a message to the Test::out stream.
  */
  static Test* current;

  /** the name of this test */
  ArduinoUnitString name;

  /** Per-test verbosity defaults to TEST_VERBOSITY_TESTS_ALL|TEST_VERBOSITY_ASSERTS_FAILED, but note that the compile-time constant TEST_VERBOSITY_MAX and run-time global (static) values Test::max_verbosity and Test::min_verbosity also effect the verbosity of a test.  According to the following rules:

    output = false;
    if (TEST_MAX_VERBOSITY bit is set (output KIND exists)) {
      if (all-test Test::max_verbosity bit is set (output KIND allowed)) {
        if (all-test Test:min_verbosity bit is set (output KIND required)) {
          output = true;
        } else if (per-test Test::verbosity bit is set (output KIND use)) {
          output = true;
        }
      }
    }

    if (output) { OUTPUT to Test::out }
  */
  uint8_t verbosity;

  /** Set state to DONE_PASS.  This does not exit the code early.  But after
      the loop() terminates, the test will be resolved and removed from the
      list of active tests. */
  void pass();


  /** Set state to DONE_FAIL.  This does not exit the code early.  But after
      the loop() terminates, the test will be resolved and removed from the
      list of active tests. */
  void fail();


  /** Set state to DONE_SKIP.  This does not exit the code early.  But after
      the loop() terminates, the test will be resolved and removed from the
      list of active tests. */
  void skip(); 

  /** Setup a test.  This is an nop {} definition by default, but for some
      more general cases it will be called once from before continuously
      calling Test::loop().  You can skip(), pass(), fail(), set verbosities, or make assertions here.*/
  virtual void setup();

  /** Run a test.  Test::loop() will be called on each Test::run() until a pass(), fail() or skip(). */
  virtual void loop() = 0;

  /** include (use) currently excluded (skipped) tests that match some
  wildcard (*) pattern like,
  
      "dev_*", "my_main_test", "*_quick"
  
  Since all tests are included by default, this is not useful except
  after an exclude() call.
  
  This should be done inside your setup() function.
  */  
  static void include(const char *pattern);

  /** 
exclude (skip) currently included tests that match some
wildcard (*) pattern like,
  
      "my_broken_test", "*_skip", "*", "io_*", etc.  
  
This should be done inside your setup() function.
  */
static void exclude(const char *pattern);

/**
  
Simple usage:

    void setup() {
      Serial.begin(9600);
    }
    
    void loop() {
      Test::run();
    }
    
Complex usage:
    
    void setup() {
      Test::exclude("*"); // exclude everything
      Test::include("io_*"); // but include io_* tests
      Test::exclude("io_*_lcd"); // except io_*_lcd tests
      Test::include("crypto_*_aes128"); // and use all crypto_*_aes128 tests
    }

void loop() {
  Test::run();
}
  */
  static void run();

  /** number of tests that have are not done (skipped, passed, or failed) */
  static int remaining();

  // Construct a test with a given name and verbosity level
  Test(const __FlashStringHelper *_name, uint8_t _verbosity = TEST_VERBOSITY_TESTS_ALL|TEST_VERBOSITY_ASSERTIONS_FAILED);

  Test(const char *_name, uint8_t _verbosity = TEST_VERBOSITY_TESTS_ALL|TEST_VERBOSITY_ASSERTIONS_FAILED);

  // Immediate test - for internal use only
  Test();

  virtual ~Test();

  static void noMessage(bool ok);

  template <typename A, typename B, typename F>
    static bool assertion(ARDUINO_UNIT_DECLARE_STRING file, uint16_t line, ARDUINO_UNIT_DECLARE_STRING lhss, const A& lhs, ARDUINO_UNIT_DECLARE_STRING ops, bool (*op)(const A& lhs, const B& rhs), ARDUINO_UNIT_DECLARE_STRING rhss, const B& rhs, const F &onMessage) {
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
      Test::out->print(ARDUINO_UNIT_STRING("Assertion "));
      Test::out->print(ok ? ARDUINO_UNIT_STRING("passed") : ARDUINO_UNIT_STRING("failed"));
      Test::out->print(ARDUINO_UNIT_STRING(": "));
      Test::out->print(ARDUINO_UNIT_STRING("("));
      Test::out->print(lhss);
      Test::out->print(ARDUINO_UNIT_STRING("="));
      Test::out->print(lhs);
      Test::out->print(ARDUINO_UNIT_STRING(") "));
      Test::out->print(ops);
      Test::out->print(ARDUINO_UNIT_STRING(" ("));
      Test::out->print(rhss);
      Test::out->print(ARDUINO_UNIT_STRING("="));
      Test::out->print(rhs);
      Test::out->print(ARDUINO_UNIT_STRING("), file "));
      Test::out->print(file);
      Test::out->print(ARDUINO_UNIT_STRING(", line "));
      Test::out->print(line);
      onMessage(ok);
      Test::out->println(ARDUINO_UNIT_STRING("."));
    }
#endif
    return ok;
  }

 /** abort all testing - all unresolved tests are resolved as failed */
 public: static void abort();
};

/** Class for creating a once-only test.  Test::run() on such a test
    calls Test::setup() once, and (if not already resolved from the
    setup(), calls Test::once() */
class TestOnce : public Test {
 public:
  TestOnce(const __FlashStringHelper * name);
  TestOnce(const char *name);
  void loop();
  virtual void once() = 0;
};

/** Create a test-once test, usually checked with assertXXXX.
    The test is assumed to pass unless failed or skipped. */
#define test(name) struct test_ ## name : TestOnce { test_ ## name() : TestOnce(ARDUINO_UNIT_STRING(#name)) {}; void once(); } test_ ## name ## _instance; void test_ ## name :: once()

/** Create an extern reference to a test-once test defined elsewhere.

This is only necessary if you use assertTestXXXX when the test
is in another file (or defined after the assertion on it). */
#define externTest(name) struct test_ ## name : TestOnce { test_ ## name(); void once(); }; extern test_##name test_##name##_instance

/** Create an extern reference to a test-until-skip-pass-or-fail test
   defined elsewhere.

This is only necessary if you use assertTestXXXX when the test
is in another file (or defined after the assertion on it). */
#define testing(name) struct test_ ## name : Test { test_ ## name() : Test(ARDUINO_UNIT_STRING(#name)) {}; void loop(); } test_ ## name ## _instance; void test_ ## name :: loop() 

/** Create an extern reference to a test-until-skip-pass-or-fail defined
elsewhere.  This is only necessary if you use assertTestXXXX when the test
is in another file (or defined after the assertion on it). */
#define externTesting(name) struct test_ ## name : Test { test_ ## name(); void loop(); }; extern test_##name test_##name##_instance

// helper define for the operators below
#define assertOpMsg(arg1,op,op_name,arg2,message,retval)                      \
  do {  if (!Test::assertion< ArduinoUnitArgTypes(arg1,arg2)::A , ArduinoUnitArgTypes(arg1,arg2)::B > (ARDUINO_UNIT_STRING(__FILE__),__LINE__,ARDUINO_UNIT_STRING(#arg1),(arg1),ARDUINO_UNIT_STRING(op_name),op,ARDUINO_UNIT_STRING(#arg2),(arg2),message)) return retval; } while (0)

#define assertOp_6(arg1,op,op_name,arg2,message,retval) assertOpMsg(arg1,op,op_name,arg2,[&](bool ok)->void { (void)ok; Test::Printer() << ARDUINO_UNIT_STRING(" [") << message << ARDUINO_UNIT_STRING("]"); },retval)

#define assertOp_5(arg1,op,op_name,arg2,message) assertOpMsg(arg1,op,op_name,arg2,[&](bool ok)->void { (void)ok; Test::Printer() << ARDUINO_UNIT_STRING(" [") << message << ARDUINO_UNIT_STRING("]"); },)
#define assertOp_4(arg1,op,op_name,arg2)         assertOpMsg(arg1,op,op_name,arg2,&Test::noMessage,)

//** macro generates optional output and calls fail() followed by a return if false. */
#define assertOp(...) ArduinoUnitMacroChoose6(assertOp_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertEqual_4(a,b,message,retval)       assertOp_6(a,compareEqual,"==",b,message,retval)
#define assertEqual_3(a,b,message)              assertOp_5(a,compareEqual,"==",b,message)
#define assertEqual_2(a,b)                      assertOp_4(a,compareEqual,"==",b)

/** macro generates optional output and calls fail() followed by a return if false. */
#define assertEqual(...) ArduinoUnitMacroChoose4(assertEqual_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertNotEqual_4(a,b,message,retval)    assertOp_6(a,compareNotEqual,"!=",b,message,retval)
#define assertNotEqual_3(a,b,message)           assertOp_5(a,compareNotEqual,"!=",b,message)
#define assertNotEqual_2(a,b)                   assertOp_4(a,compareNotEqual,"!=",b)

/** macro generates optional output and calls fail() followed by a return if false. */
#define assertNotEqual(...) ArduinoUnitMacroChoose4(assertNotEqual_, ## __VA_ARGS__)(__VA_ARGS__)


#define assertLess_4(a,b,message,retval)        assertOp_6(a,compareLess,"<",b,message,retval)
#define assertLess_3(a,b,message)               assertOp_5(a,compareLess,"<",b,message)
#define assertLess_2(a,b)                       assertOp_4(a,compareLess,"<",b)
/** macro generates optional output and calls fail() followed by a return if false. */
#define assertLess(...) ArduinoUnitMacroChoose4(assertLess_, ## __VA_ARGS__)(__VA_ARGS__)


#define assertMore_4(a,b,message,retval)        assertOp_6(a,compareMore,">",b,message,retval)
#define assertMore_3(a,b,message)               assertOp_5(a,compareMore,">",b,message)
#define assertMore_2(a,b)                       assertOp_4(a,compareMore,">",b)
/** macro generates optional output and calls fail() followed by a return if false. */
#define assertMore(...) ArduinoUnitMacroChoose4(assertMore_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertLessOrEqual_4(a,b,message,retval) assertOp_6(a,compareLessOrEqual,"<=",b,message,retval)
#define assertLessOrEqual_3(a,b,message)        assertOp_5(a,compareLessOrEqual,"<=",b,message)
#define assertLessOrEqual_2(a,b)                assertOp_4(a,compareLessOrEqual,"<=",b)
/** macro generates optional output and calls fail() followed by a return if false. */
#define assertLessOrEqual(...) ArduinoUnitMacroChoose4(assertLessOrEqual_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertMoreOrEqual_4(a,b,message,retval) assertOp_6(a,compareMoreOrEqual,">=",b,message,retval)
#define assertMoreOrEqual_3(a,b,message)        assertOp_5(a,compareMoreOrEqual,">=",b,message)
#define assertMoreOrEqual_2(a,b)                assertOp_4(a,compareMoreOrEqual,">=",b)
/** macro generates optional output and calls fail() followed by a return if false. */
#define assertMoreOrEqual(...) ArduinoUnitMacroChoose4(assertMoreOrEqual_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertTrue_3(a,message,retval)          assertOp_6(a,compareEqual,"==",true,message,retval)
#define assertTrue_2(a,message)                 assertOp_5(a,compareEqual,"==",true,message)
#define assertTrue_1(a)                         assertOp_4(a,compareEqual,"==",true)
/** macro generates optional output and calls fail() followed by a return if false. */
#define assertTrue(...) ArduinoUnitMacroChoose3(assertTrue_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertFalse_3(a,message,retval)         assertOp_6(a,compareEqual,"==",false,message,retval)
#define assertFalse_2(a,message)                assertOp_5(a,compareEqual,"==",false,message)
#define assertFalse_1(a)                        assertOp_4(a,compareEqual,"==",false)
/** macro generates optional output and calls fail() followed by a return if false. */
#define assertFalse(...) ArduinoUnitMacroChoose3(assertFalse_, ## __VA_ARGS__)(__VA_ARGS__)

#define checkTestDone(name) (test_##name##_instance.state >= Test::DONE_SKIP)

/** check condition only */
#define checkTestNotDone(name) (test_##name##_instance.state < Test::DONE_SKIP)

/** check condition only */
#define checkTestPass(name) (test_##name##_instance.state == Test::DONE_PASS)

/** check condition only */
#define checkTestNotPass(name) (test_##name##_instance.state != Test::DONE_PASS)

/** check condition only */
#define checkTestFail(name) (test_##name##_instance.state == Test::DONE_FAIL)

/** check condition only */
#define checkTestNotFail(name) (test_##name##_instance.state != Test::DONE_FAIL)

/** check condition only */
#define checkTestSkip(name) (test_##name##_instance.state == Test::DONE_SKIP)

/** check condition only */
#define checkTestNotSkip(name) (test_##name##_instance.state != Test::DONE_SKIP)

#define assertTestDone_3(name,message,retval)     \
  assertOp_6(test_##name##_instance.state,compareMoreOrEqual,">=",Test::DONE_SKIP,message,retval)
#define assertTestDone_2(name,message)            \
  assertOp_5(test_##name##_instance.state,compareMoreOrEqual,">=",Test::DONE_SKIP,message)
#define assertTestDone_1(name)                    \
  assertOp_4(test_##name##_instance.state,compareMoreOrEqual,">=",Test::DONE_SKIP)
/** macro generates optional output and calls fail() followed by a return if false. */
#define assertTestDone(...) ArduinoUnitMacroChoose3(assertTestDone_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertTestNotDone_3(name,message,retval)  \
  assertOp_6(test_##name##_instance.state,compareLess,"<",Test::DONE_SKIP,message,retval)
#define assertTestNotDone_2(name,message)         \
  assertOp_5(test_##name##_instance.state,compareLess,"<",Test::DONE_SKIP,message)
#define assertTestNotDone_1(name)                 \
  assertOp_4(test_##name##_instance.state,compareLess,"<",Test::DONE_SKIP)
/** macro generates optional output and calls fail() followed by a return if false. */
#define assertTestNotDone(...) ArduinoUnitMacroChoose3(assertTestNotDone_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertTestPass_3(name,message,retval)     \
  assertOp_6(test_##name##_instance.state,compareEqual,"==",Test::DONE_PASS,message,retval)
#define assertTestPass_2(name,message)            \
  assertOp_5(test_##name##_instance.state,compareEqual,"==",Test::DONE_PASS,message)
#define assertTestPass_1(name)                    \
  assertOp_4(test_##name##_instance.state,compareEqual,"==",Test::DONE_PASS)
/** macro generates optional output and calls fail() followed by a return if false. */
#define assertTestPass(...) ArduinoUnitMacroChoose3(assertTestPass_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertTestNotPass_3(name,message,retval)                        \
  assertOp_6(test_##name##_instance.state,compareNotEqual,"!=",Test::DONE_PASS,message,retval)
#define assertTestNotPass_2(name,message)                               \
  assertOp_5(test_##name##_instance.state,compareNotEqual,"!=",Test::DONE_PASS,message)
#define assertTestNotPass_1(name)                                       \
  assertOp_4(test_##name##_instance.state,compareNotEqual,"!=",Test::DONE_PASS)
/** macro generates optional output and calls pass() followed by a return if false. */
#define assertTestNotPass(...) ArduinoUnitMacroChoose3(assertTestNotPass_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertTestFail_3(name,message,retval)                           \
  assertOp_6(test_##name##_instance.state,compareEqual,"==",Test::DONE_FAIL,message,retval)  
#define assertTestFail_2(name,message)                                  \
  assertOp_5(test_##name##_instance.state,compareEqual,"==",Test::DONE_FAIL,message)
#define assertTestFail_1(name)                                          \
  assertOp_4(test_##name##_instance.state,compareEqual,"==",Test::DONE_FAIL)
/** macro generates optional output and calls fail() followed by a return if false. */
#define assertTestFail(...) ArduinoUnitMacroChoose3(assertTestFail_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertTestNotFail_3(name,message,retval)                        \
  assertOp_6(test_##name##_instance.state,compareNotEqual,"!=",Test::DONE_FAIL,message,retval)
#define assertTestNotFail_2(name,message)                               \
  assertOp_5(test_##name##_instance.state,compareNotEqual,"!=",Test::DONE_FAIL,message)
#define assertTestNotFail_1(name)                                       \
  assertOp_4(test_##name##_instance.state,compareNotEqual,"!=",Test::DONE_FAIL)
/** macro generates optional output and calls fail() followed by a return if false. */
#define assertTestNotFail(...) ArduinoUnitMacroChoose3(assertTestNotFail_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertTestSkip_3(name,message,retval)                                 \
  assertOp_6(test_##name##_instance.state,compareEqual,"==",Test::DONE_SKIP,message,retval)
#define assertTestSkip_2(name,message)        \
  assertOp_5(test_##name##_instance.state,compareEqual,"==",Test::DONE_SKIP,message)
#define assertTestSkip_1(name)                \
  assertOp_4(test_##name##_instance.state,compareEqual,"==",Test::DONE_SKIP)
/** macro generates optional output and calls skip() followed by a return if false. */
#define assertTestSkip(...) ArduinoUnitMacroChoose3(assertTestSkip_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertTestNotSkip_3(name,message,retval)                              \
  assertOp_6(test_##name##_instance.state,compareNotEqual,"!=",Test::DONE_SKIP,message,retval)
#define assertTestNotSkip_2(name,message) \
  assertOp_5(test_##name##_instance.state,compareNotEqual,"!=",Test::DONE_SKIP,message)
#define assertTestNotSkip_1(name) \
  assertOp_4(test_##name##_instance.state,compareNotEqual,"!=",Test::DONE_SKIP)
/** macro generates optional output and calls skip() followed by a return if false. */
#define assertTestNotSkip(...) ArduinoUnitMacroChoose3(assertTestNotSkip_, ## __VA_ARGS__)(__VA_ARGS__)

#define checkCurrentTestDone() (Test::current->state >= Test::DONE_SKIP)

/** check condition only */
#define checkCurrentTestNotDone() (Test::current->state < Test::DONE_SKIP)

/** check condition only */
#define checkCurrentTestPass() (Test::current->state == Test::DONE_PASS)

/** check condition only */
#define checkCurrentTestNotPass() (Test::current->state != Test::DONE_PASS)

/** check condition only */
#define checkCurrentTestFail() (Test::current->state == Test::DONE_FAIL)

/** check condition only */
#define checkCurrentTestNotFail() (Test::current->state != Test::DONE_FAIL)

/** check condition only */
#define checkCurrentTestSkip() (Test::current->state == Test::DONE_SKIP)

/** check condition only */
#define checkCurrentTestNotSkip() (Test::current->state != Test::DONE_SKIP)

#define assertCurrentTestDone_2(message,retval)        assertOp_6(Test::current->state,compareMoreOrEqual,">=",Test::DONE_SKIP,message,retval)
#define assertCurrentTestDone_1(message)        assertOp_5(Test::current->state,compareMoreOrEqual,">=",Test::DONE_SKIP,message)
#define assertCurrentTestDone_0()                assertOp_4(Test::current->state,compareMoreOrEqual,">=",Test::DONE_SKIP)
/** macro generates optional output and calls fail() followed by a return if false. */
#define assertCurrentTestDone(...) ArduinoUnitMacroChoose2(assertCurrentTestDone_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertCurrentTestNotDone_2(message,retval)        assertOp_6(Test::current->state,compareLess,"<",Test::DONE_SKIP,message,retval)
#define assertCurrentTestNotDone_1(message)        assertOp_5(Test::current->state,compareLess,"<",Test::DONE_SKIP,message)
#define assertCurrentTestNotDone_0()                assertOp_4(Test::current->state,compareLess,"<",Test::DONE_SKIP)
/** macro generates optional output and calls fail() followed by a return if false. */
#define assertCurrentTestNotDone(...) ArduinoUnitMacroChoose2(assertCurrentTestNotDone_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertCurrentTestPass_2(message,retval)        assertOp_6(Test::current->state,compareEqual,"==",Test::DONE_PASS,message,retval)
#define assertCurrentTestPass_1(message)        assertOp_5(Test::current->state,compareEqual,"==",Test::DONE_PASS,message)
#define assertCurrentTestPass_0()                assertOp_4(Test::current->state,compareEqual,"==",Test::DONE_PASS)
/** macro generates optional output and calls fail() followed by a return if false. */
#define assertCurrentTestPass(...) ArduinoUnitMacroChoose2(assertCurrentTestPass_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertCurrentTestNotPass_2(message,retval)        assertOp_6(Test::current->state,compareNotEqual,"!=",Test::DONE_PASS,message,retval)
#define assertCurrentTestNotPass_1(message)        assertOp_5(Test::current->state,compareNotEqual,"!=",Test::DONE_PASS,message)
#define assertCurrentTestNotPass_0()                assertOp_4(Test::current->state,compareNotEqual,"!=",Test::DONE_PASS)
/** macro generates optional output and calls pass() followed by a return if false. */
#define assertCurrentTestNotPass(...) ArduinoUnitMacroChoose2(assertCurrentTestNotPass_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertCurrentTestFail_2(message,retval)        assertOp_6(Test::current->state,compareEqual,"==",Test::DONE_FAIL,message,retval)  
#define assertCurrentTestFail_1(message)        assertOp_5(Test::current->state,compareEqual,"==",Test::DONE_FAIL,message)
#define assertCurrentTestFail_0()                assertOp_4(Test::current->state,compareEqual,"==",Test::DONE_FAIL)
/** macro generates optional output and calls fail() followed by a return if false. */
#define assertCurrentTestFail(...) ArduinoUnitMacroChoose2(assertCurrentTestFail_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertCurrentTestNotFail_2(message,retval)        assertOp_6(Test::current->state,compareNotEqual,"!=",Test::DONE_FAIL,message,retval)
#define assertCurrentTestNotFail_1(message)        assertOp_5(Test::current->state,compareNotEqual,"!=",Test::DONE_FAIL,message)
#define assertCurrentTestNotFail_0()                assertOp_4(Test::current->state,compareNotEqual,"!=",Test::DONE_FAIL)
/** macro generates optional output and calls fail() followed by a return if false. */
#define assertCurrentTestNotFail(...) ArduinoUnitMacroChoose2(assertCurrentTestNotFail_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertCurrentTestSkip_2(message,retval)        assertOp_6(Test::current->state,compareEqual,"==",Test::DONE_SKIP,message,retval)
#define assertCurrentTestSkip_1(message)        assertOp_5(Test::current->state,compareEqual,"==",Test::DONE_SKIP,message)
#define assertCurrentTestSkip_0()                assertOp_4(Test::current->state,compareEqual,"==",Test::DONE_SKIP)
/** macro generates optional output and calls skip() followed by a return if false. */
#define assertCurrentTestSkip(...) ArduinoUnitMacroChoose2(assertCurrentTestSkip_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertCurrentTestNotSkip_2(message,retval)        assertOp_6(Test::current->state,compareNotEqual,"!=",Test::DONE_SKIP,message,retval)
#define assertCurrentTestNotSkip_1(message)        assertOp_5(Test::current->state,compareNotEqual,"!=",Test::DONE_SKIP,message)
#define assertCurrentTestNotSkip_0()                assertOp_4(Test::current->state,compareNotEqual,"!=",Test::DONE_SKIP)
/** macro generates optional output and calls skip() followed by a return if false. */
#define assertCurrentTestNotSkip(...) ArduinoUnitMacroChoose2(assertCurrentTestNotSkip_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertNearMsg(a,b,max,message,retval)                                  \
  do {  double ArduinoUnitA=(a), ArduinoUnitB=(b), ArduinoUnitMax=(max), ArduinoUnitErr=fabs(ArduinoUnitA-ArduinoUnitB); if (!Test::assertion< double, double > (ARDUINO_UNIT_STRING(__FILE__),__LINE__,ARDUINO_UNIT_STRING("abserr"),ArduinoUnitErr,ARDUINO_UNIT_STRING("<="),compareLessOrEqual,ARDUINO_UNIT_STRING(#max),ArduinoUnitMax,[&](bool ok)->void { (void)ok; Test::Printer() << ARDUINO_UNIT_STRING(" [(") << ARDUINO_UNIT_STRING(#a) << ARDUINO_UNIT_STRING("=") << ArduinoUnitA << ARDUINO_UNIT_STRING(") ~ (") << ARDUINO_UNIT_STRING(#b) << ARDUINO_UNIT_STRING("=") << ArduinoUnitB << ARDUINO_UNIT_STRING(")") << message << ARDUINO_UNIT_STRING("]"); })) return retval; } while (0)

#define assertNear_5(a,b,max,message,retval)                            \
  assertNearMsg(a,b,max,ARDUINO_UNIT_STRING(" ") << message,retval)

#define assertNear_4(a,b,max,message)                                   \
  assertNearMsg(a,b,max,ARDUINO_UNIT_STRING(" ") << message,)

#define assertNear_3(a,b,max)                                           \
  assertNearMsg(a,b,max,ARDUINO_UNIT_STRING(""),)

#define assertNear(...) ArduinoUnitMacroChoose5(assertNear_, ## __VA_ARGS__)(__VA_ARGS__)

#define assertRelativelyNearMsg(a,b,max,message,retval)                                  \
  do {  double ArduinoUnitA=(a), ArduinoUnitB=(b), ArduinoUnitMax=(max), ArduinoUnitErr=fabs(ArduinoUnitA-ArduinoUnitB)/(0.5*(fabs(ArduinoUnitA)+fabs(ArduinoUnitB))); if (!Test::assertion< double, double > (ARDUINO_UNIT_STRING(__FILE__),__LINE__,ARDUINO_UNIT_STRING("relerr"),ArduinoUnitErr,ARDUINO_UNIT_STRING("<="),compareLessOrEqual,ARDUINO_UNIT_STRING(#max),ArduinoUnitMax,[&](bool ok)->void { (void)ok; Test::Printer() << ARDUINO_UNIT_STRING(" [(") << ARDUINO_UNIT_STRING(#a) << ARDUINO_UNIT_STRING("=") << ArduinoUnitA << ARDUINO_UNIT_STRING(") ~ (") << ARDUINO_UNIT_STRING(#b) << ARDUINO_UNIT_STRING("=") << ArduinoUnitB << ARDUINO_UNIT_STRING(")") << message << ARDUINO_UNIT_STRING("]"); })) return retval; } while (0)

#define assertRelativelyNear_5(a,b,max,message,retval)                            \
  assertRelativelyNearMsg(a,b,max,ARDUINO_UNIT_STRING(" ") << message,retval)

#define assertRelativelyNear_4(a,b,max,message)                                   \
  assertRelativelyNearMsg(a,b,max,ARDUINO_UNIT_STRING(" ") << message,)

#define assertRelativelyNear_3(a,b,max)                                           \
  assertRelativelyNearMsg(a,b,max,ARDUINO_UNIT_STRING(""),)

#define assertRelativelyNear(...) ArduinoUnitMacroChoose5(assertRelativelyNear_, ## __VA_ARGS__)(__VA_ARGS__)

#if defined(ARDUINO)
#define ARDUINO_UNIT_TESTS                                    \
  void setup() { Serial.begin(115200L); while (!Serial) {}; } \
  void loop() { Test::run(); }
#else
#define ARDUINO_UNIT_TESTS                                              \
  int main(int argc, const char *argv[]) {                              \
    for (int i=1; i<argc; ++i) {                                        \
      if (strcmp(argv[i],"-i")==0 || strcmp(argv[i],"--include")==0) { Test::include(argv[++i]); } \
      if (strcmp(argv[i],"-e")==0 || strcmp(argv[i],"--exclude")==0) { Test::exclude(argv[++i]); } \
    }                                                                   \
    do { Test::run(); } while (Test::remaining() > 0);                  \
    return Test::getCurrentFailed() == 0 ? 0 : 1;                       \
  }
#endif

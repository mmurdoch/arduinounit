#pragma once

/**

  @file ArduinoUnit.h

*/

#include <stdint.h>
#include <Print.h>

#if ARDUINO >= 100 && ARDUINO < 103
#undef F
#undef PSTR
#define PSTR(s) (__extension__({static const char __c[] PROGMEM = (s); &__c[0];}))

#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))

#endif

#if defined(__GNUC__) && (__GNUC__*100 + __GNUC_MINOR__ < 407)
// Workaround for http://gcc.gnu.org/bugzilla/show_bug.cgi?id=34734
//
#ifdef PROGMEM
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))
#endif
#endif

// Workaround for Arduino Due
#if defined(__arm__) && !defined(PROGMEM)
#define PROGMEM
#define PSTR(s) s
#define memcpy_P(a, b, c) memcpy(a, b, c)
#define strlen_P(a) strlen(a)
#endif

#include "ArduinoUnitUtility/Compare.h"
#include "ArduinoUnitUtility/FakeStream.h"
#include "ArduinoUnitUtility/FakeStreamBuffer.h"
#include "ArduinoUnitUtility/FreeMemory.h"

/** \brief This is defined to manage the API transition to 2.X */
#define ARDUINO_UNIT_MAJOR_VERSION 2

/** \brief This is defined to manage the API transition to 2.X */
#define ARDUINO_UNIT_MINOR_VERSION 0

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
  // allows for both ram/progmem based names
  class TestString : public Printable {
  public:
    const uint32_t data;
    TestString(const char *_data);
    TestString(const __FlashStringHelper *_data);
    void read(void *destination, uint16_t offset, uint8_t length) const;
    uint16_t length() const;
    int8_t compare(const Test::TestString &to) const;
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
  TestString name;

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

  // Construct a test with a given name and verbosity level
  Test(const __FlashStringHelper *_name, uint8_t _verbosity = TEST_VERBOSITY_TESTS_ALL|TEST_VERBOSITY_ASSERTIONS_FAILED);

  Test(const char *_name, uint8_t _verbosity = TEST_VERBOSITY_TESTS_ALL|TEST_VERBOSITY_ASSERTIONS_FAILED);

  virtual ~Test();

  template <typename A, typename B>
    static bool assertion(const __FlashStringHelper *file, uint16_t line, const __FlashStringHelper *lhss, const A& lhs, const __FlashStringHelper *ops, bool (*op)(const A& lhs, const B& rhs), const __FlashStringHelper *rhss, const B& rhs) {
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
      out->print(F("Assertion "));
      out->print(ok ? F("passed") : F("failed"));
      out->print(F(": ("));
      out->print(lhss);
      out->print(F("="));
      out->print(lhs);
      out->print(F(") "));
      out->print(ops);
      out->print(F(" ("));
      out->print(rhss);
      out->print(F("="));
      out->print(rhs);
      out->print(F("), file "));
      out->print(file);
      out->print(F(", line "));
      out->print(line);
      out->println(".");
    }
#endif
    return ok;
  }
};

/** Class for creating a once-only test.  Test::run() on such a test
    calls Test::setup() once, and (if not already resolved from the
    setup(), calls Test::once() */
class TestOnce : public Test {
 public:
  TestOnce(const __FlashStringHelper *name);
  TestOnce(const char *name);
  void loop();
  virtual void once() = 0;
};

/** Class to unify comparisons.  There are a variety of specializations to account for
    char *, const char *, and char [N] types which map to strcmp(). 
*/


/** Create a test-once test, usually checked with assertXXXX.
    The test is assumed to pass unless failed or skipped. */
#define test(name) struct test_ ## name : TestOnce { test_ ## name() : TestOnce(F(#name)) {}; void once(); } test_ ## name ## _instance; void test_ ## name :: once() 

/** Create an extern reference to a test-once test defined elsewhere.

This is only necessary if you use assertTestXXXX when the test
is in another file (or defined after the assertion on it). */
#define externTest(name) struct test_ ## name : TestOnce { test_ ## name(); void once(); }; extern test_##name test_##name##_instance

/** Create an extern reference to a test-until-skip-pass-or-fail test
   defined elsewhere.

This is only necessary if you use assertTestXXXX when the test
is in another file (or defined after the assertion on it). */
#define testing(name) struct test_ ## name : Test { test_ ## name() : Test(F(#name)) {}; void loop(); } test_ ## name ## _instance; void test_ ## name :: loop() 

/** Create an extern reference to a test-until-skip-pass-or-fail defined
elsewhere.  This is only necessary if you use assertTestXXXX when the test
is in another file (or defined after the assertion on it). */
#define externTesting(name) struct test_ ## name : Test { test_ ## name(); void loop(); }; extern test_##name test_##name##_instance

// helper define for the operators below
#define assertOp(arg1,op,op_name,arg2) do { if (!Test::assertion<typeof(arg1),typeof(arg2)>(F(__FILE__),__LINE__,F(#arg1),(arg1),F(op_name),op,F(#arg2),(arg2))) return; } while (0)

/** macro generates optional output and calls fail() followed by a return if false. */
#define assertEqual(arg1,arg2)       assertOp(arg1,compareEqual,"==",arg2)

/** macro generates optional output and calls fail() followed by a return if false. */
#define assertNotEqual(arg1,arg2)    assertOp(arg1,compareNotEqual,"!=",arg2)

/** macro generates optional output and calls fail() followed by a return if false. */
#define assertLess(arg1,arg2)        assertOp(arg1,compareLess,"<",arg2)

/** macro generates optional output and calls fail() followed by a return if false. */
#define assertMore(arg1,arg2)        assertOp(arg1,compareMore,">",arg2)

/** macro generates optional output and calls fail() followed by a return if false. */
#define assertLessOrEqual(arg1,arg2) assertOp(arg1,compareLessOrEqual,"<=",arg2)

/** macro generates optional output and calls fail() followed by a return if false. */
#define assertMoreOrEqual(arg1,arg2) assertOp(arg1,compareMoreOrEqual,">=",arg2)

/** macro generates optional output and calls fail() followed by a return if false. */
#define assertTrue(arg) assertEqual(arg,true)

/** macro generates optional output and calls fail() followed by a return if false. */
#define assertFalse(arg) assertEqual(arg,false)

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

/** macro generates optional output and calls fail() followed by a return if false. */
#define assertTestDone(name) assertMoreOrEqual(test_##name##_instance.state,Test::DONE_SKIP)
/** macro generates optional output and calls fail() followed by a return if false. */
#define assertTestNotDone(name) assertLess(test_##name##_instance.state,Test::DONE_SKIP)

/** macro generates optional output and calls fail() followed by a return if false. */
#define assertTestPass(name) assertEqual(test_##name##_instance.state,Test::DONE_PASS)

/** macro generates optional output and calls fail() followed by a return if false. */
#define assertTestNotPass(name) assertNotEqual(test_##name##_instance.state,Test::DONE_PASS)

/** macro generates optional output and calls fail() followed by a return if false. */
#define assertTestFail(name) assertEqual(test_##name##_instance.state,Test::DONE_FAIL)

/** macro generates optional output and calls fail() followed by a return if false. */
#define assertTestNotFail(name) assertNotEqual(test_##name##_instance.state,Test::DONE_FAIL)

/** macro generates optional output and calls fail() followed by a return if false. */
#define assertTestSkip(name) assertEqual(test_##name##_instance.state,Test::DONE_SKIP)

/** macro generates optional output and calls fail() followed by a return if false. */
#define assertTestNotSkip(name) assertNotEqual(test_##name##_instance.state,Test::DONE_SKIP)

ArduinoUnit
===========

Unit test framework for arduino projects.

###Version 2.0

##Why

This follows the spirit of ArduinoUnit 1.X with the following
less-is-more features:

1. It does not use dynamic memory.
1. It does not break tests into suites.
1. It does not have custom reports.

And the following more-is-more features:

1. A wider variety of assertions and assertion argument types.
1. A wildcard include() and exclude() mechanism for
   selecting active tests.
1. A per-test verbosity setting.
1. Check-once tests (like ArduinoUnit), but also 
   check-until-skip-pass-or-fail and class structured 
   (TestOnce and Test) tests.
1. The output destination and verbosity is quite flexible.
1. Test names and assert strings are stored in flash (not RAM).
   - Test names can optionally be stored in either RAM or flash.
1. assertions about other tests.

##Getting Started

Create a directory called ArduinoUnit under `<arduino installation directory>\libraries`.

Copy everything from the `src` directory to this new directory.

Open a sketch in arduino and click `Sketch | Import Library... | ArduinoUnit` to start unit testing.

For example, try this simple unit testing sketch:

```
#line 2 "sketch"
#include <ArduinoUnit.h>

test(ok) 
{
  int x=3;
  int y=3;
  assertEqual(x,y);
}

test(bad)
{
  int x=3;
  int y=3;
  assertNotEqual(x,y);
}

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Test::run();
}
```

Upload this sketch to the Arduino (using the 'Upload' button, `File | Upload` or `Ctrl+U`).

Turn on the Serial Monitor (using the 'Serial Monitor' button, `Tools | Serial Monitor` or 
  `Ctrl+Shift+M`) and expect to see the following:

```
sketch:17:1: fail assert (x=3) != (y=3)
test bad failed.
test ok passed.
test summary: 1 passed, 1 failed, and 0 skipped, out of 2 test(s).
```
#Verbosity

Just how much information is generated on each test is fairly flexible, and designed to address these rules:

1. How much code is generated (TEST_MAX_VERBOSITY)
1. Global maximums subject to 1 (static Test::max_verbosity)
1. Global minimums subject to 1 and 2 (static Test::min_verbosity)
1. Per-test requirements subject to 1, 2 and 3 (Test::verbosity)

The rules are as follows for each kind of possible output flag:

1. Is the flag set in TEST_MAX_VERBOSITY?  If not set, then the output is suppressed and we are done.
1. Is the flag set in Test::max_verbosity? If not set, then the output is suppressed and we are done.
1. Is the flag set in Test::min_verbosity? If set, then the output is generated and we are done.
1. Are we in a test context (Test::run())?

    If so, the output is generated if the corresponding
    per-test verbosity flag is set.

The default values are as follows

```
   TEST_MAX_VERBOSITY = TEST_VERBOSITY_ALL

   static Test::max_verbosity = TEST_VERBOSITY_ALL
   static Test::min_verbosity = TEST_VERBOSITY_TESTS_SUMMARY

   Test::verbosity = (TEST_VERBOSTY_TESTS_ALL|TEST_VERBOSITY_ASSERTIONS_FAILED)
```

This amounts to asking for a summary of each test (skip, pass, fail), an overall summary when all tests are resolved, and a more detailed report on each of the failed assertions.  

The verbosity flags are the bitwise-or of the following values

```
TEST_VERBOSITY_TESTS_SUMMARY      (0x01)
TEST_VERBOSITY_TESTS_FAILED       (0x02)
TEST_VERBOSITY_TESTS_PASSED       (0x04)
TEST_VERBOSITY_TESTS_SKIPPED      (0x08)
TEST_VERBOSITY_TESTS_ALL          (0x0F)
TEST_VERBOSITY_ASSERTIONS_FAILED  (0x10)
TEST_VERBOSITY_ASSERTIONS_PASSED  (0x20)
TEST_VERBOSITY_ASSERTIONS_ALL     (0x30)
TEST_VERBOSITY_ALL                (0x3F)
TEST_VERBOSITY_NONE               (0x00)
```

#Output

The `Test::out` value is the *shared* value for all tests describing where output for all tests goes.  The default is 

```
Test::out = &Serial;
```

But you can set to the address of any Print stream, for example, if you want the output to be on the `Serial3` device on the arduino mega, use

```
Test::out = &Serial3;
```

in your `setup()`.  Note the library does not set the baud rate - you have to do that in your `setup()`.

##Built-in Assertions

The following assertions are supported

```
assertLess(arg1,arg2)
assertLessOrEqual(arg1,arg2)
assertEqual(arg1,arg2)
assertNotEqual(arg1,arg2)
assertMoreOrEqual(arg1,arg2)
assertMore(arg1,arg2)
```
The following types are supported for these assertions:
```
String
char *
unsigned char
int
unsigned int
long
unsigned long
double
```
There are addtionally some boolean assertions:
```
assertTrue(arg)
assertFalse(arg)
```
See the section below for assertions on tests.

The output from these assertions is to print a string represenation of the
arguments, and the value of the arguments, as in:
```
file:line:column: pass/fail assert (arg1=value1) op (arg2=value2)
```
These assertions are defined in a way that the problem of multiple
evaluations is avoided.  The arguments are only evaluated once in these
assertions.  

All the assert macros expand to a test that creates an optional message, and, if false, calls fail() on the current test and returns.

##Meta Assertions

You can make assertions on the outcome of tests as well.  The following meta-assertions are supported:
```
assertTestDone(test)
assertTestNotDone(test)
assertTestPass(test)
assertTestNotPass(test)
assertTestFail(test)
assertTestNotFail(test)
assertTestSkip(test)
assertTestNotSkip(test)
```
These can be used in conjunction with the boolean check-only macros
```
checkTestDone(test)
checkTestNotDone(test)
checkTestPass(test)
checkTestNotPass(test)
checkTestFail(test)
checkTestNotFail(test)
checkTestSkip(test)
checkTestNotSkip(test)
```
These behave like the other asserts, but they work only in the context
of other tests.  The most likely place you would have such a test would
be in a testing meta-test as so:

```
test(ok) { pass(); }
test(bad) { fail(); }
testing(slow) { if (millis() > 1000) pass(); }

testing(passed) 
{
  if (checkTestDone(ok)) {
    assertTestPass(ok);
    pass();
  }
}

testing(too_slow)
{
  if (millis() > 100) {
    assertTestDone(slow);
    pass();
  }
}
```
Since the ordering tests cannot be controlled, only use test-asserts
in a testing() environment.

#`Test` and `TestOnce`
You can create your own modular tests by deriving from these classes.

```
class MyTest : public Test {
public:
  MyTest(const __FlashStringHelper *name) : Test(name) {
    // TODO: construct named test.
    // This should be lightweight - it may be excluded
    //
    // You can set verbosity.
  }
  void setup() {
    // TODO: setup test
    // You can call pass(), fail(), or skip() to immediately resolve test
    // You can make assertions.
    // You can set verbosity.
  }
  void loop() {
    // TODO: run test on each loop
    // You can call pass(), fail(), or skip() to resolve test
    // You can make assertions.
    // You can set verbosity.
  }
};

class MyTestOnce : public TestOnce
{
public:
  MyTestOnce(const __FlashStringHelper *name) : TestOnce(name) {
  // same as MyTest
  }
  void setup() {
  // same as MyTest
  }
  void once() {
  // same as MyTest::loop(), but will only be called once from loop()
  // if included in the active tests and was not resolved in setup().
  }
  
}

// create instances of the custom test 
MyTest myTest1(F("myTest1"));
MyTest myTest2(F("myTest2"));

MyTestOnce myTestOnce1(F("myTestOnce1"));
MyTestOnce myTestOnce2(F("myTestOnce2"));
```

Note that `Test::run()` only calls the active unresolved tests.

##Selecting tests

In your setup() function, you can select which tests are going to be setup and looped.  The default is that all tests are included.

`Test::exclude(const char *pattern)` removes all the tests that match the given *-pattern.

`Test::include(const char *pattern)` includes all the tests that match the given *-pattern.

Here are some examples:

##Select examples:

A single test `my_test`

```
void setup()
{
  Serial.begin(9600);
  Test::exclude("*");
  Test::include("my_test");
}
```

All tests named dev_-something, but not the ones ending in _skip or _slow, or have the word eeprom in them:
```
void setup()
{
  Serial.begin(9600);
  Test::exclude("*");
  Test::include("dev_*");
  Test::exclude("*_slow");
  Test::exclude("*_skip");
  Test::exclude("*eeprom*");
}
```

##FAQ

Q. The line number of the asserts do not match the source file.

A.  As far as I can tell, this is a bug in the compiler -- look two
   lines up.  I do not know why the `__LINE__` macro does not match
   the actual line of code.

Q. I get these link errors about multiply defined test_XXXX_instance.

A. You have defined two tests with the same name XXXX using either the
   test() or testing() macro.

Q. I get no output

A. Here is a troubleshooting guideline:

 * Make sure you call `Serial.begin()` in your setup.  Or, if you redirect
   output by changing the value of `Test::out`, make sure you configure
   the Print stream you direct it to.
 * Make sure you call `Test::run()` in your loop().
 * Make sure you did not exclude the test(s) with `Test::exclude(pattern)`.
   By default all tests are included.
 * Make sure your tests complete.
   * Each single-check test() test not be in an infinite loop.
   * Each continous testing() test do a small amount of work on each
     call, and should eventually invoke pass(), fail() or skip().
 * Make sure verbosity is adequate.  You can generate all possible output by
   * Assuring that TEST_MAX_VERBOSITY in ArduinoUnit.h is TEST_VERBOSITY_ALL (the default).
   * Assuring that Test::max_verbosity is TEST_VERBOSITY_ALL (the default).
   * Setting Test::min_verbosity = TEST_VERBOSITY_ALL (the default is TEST_VERSOBITY_TESTS_ALL | TEST_VERBOSITY_ASSERTIONS_FAILED, generating output only for failed assertions, completions of tests, and an overall summary).
   * With these settings, the per-test verbosity has no effect.

##License

Copyright (c) 2013 Warren MacEvoy, Matthew Murdoch

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

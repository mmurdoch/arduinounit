ArduinoUnit
===========

ArduinoUnit is a testing framework for Arduino projects. It supports Arduino, ESP8266 and ESP32 as well as "en vitro" development system (vs embedded target) testing.

## Getting Started

Install the library from the Arduino IDE.  From the menu, navigate:

* Sketch->Include Library->Manage Libraries...
* Search for "arduinounit"
* Install

After this, examples should be available from File->Examples in the "Examples from Custom Libraries" section.

Here is a simple unit testing sketch:

```
#line 2 "sketch.ino"
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
  while(!Serial) {} // Portability for Leonardo/Micro
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
Assertion failed: (x=3) != (y=3), file sketch.ino, line 17.
Test bad failed.
Test ok passed.
Test summary: 1 passed, 1 failed, and 0 skipped, out of 2 test(s).
```

The following asserts are supported [with an optional footnote and return value]

| Assertion | Description |
| --- | --- |
| `assertEqual(a,b [,footnote [,retval]])` | `a == b`? |
| `assertNear(a,b,maxerr, [,footnote[,retval]])` | `abs(b-a)<=maxerr`? |
| `assertRelativelyNear(a,b,maxerr, [,footnote[,retval]])` | `abs(b-a)/abs(½(abs(a)+abs(b)))<=maxerr`? |
| `assertNotEqual(a,b [,footnote[,retval]])` | `a != b`? |
| `assertLess(a,b [,footnote[,retval]])` | `a < b`? |
| `assertLessOrEqual(a,b [,footnote[,retval]])` | `a <= b`? |
| `assertMore(a,b [,footnote[,retval]])` | `a > b`? |
| `assertMoreOrEqual(a,b [,footnote[,retval]])` | `a >= b`? |
| `assertTrue(p [,footnote[,retval]])` | same as `assertEqual(p,true)` |
| `assertFalse(p [,footnote[,retval]])` | same as `assertEqual(p,false)` |


## [,footnote[,retval]]

Assertions are replaced with essentially (retval is the optional last value of an assert):

    if (not assertion) { fail(); return [retval]; }

When things go wrong, it can be useful to print additional information.  As of 2.3.2-alpha, this is possible with any assertXXX() method by adding an additional third parameter [footnote] to the assert.  For example,
```
test(cases)
{
  int x=3;
  for (int k=0; k<4; ++k) {
    assertNotEqual(x,k,"case k=" << k);
  }
}
```
will fail with the message
```
Assertion failed: (x=3) != (k=3), file basic.ino, line 20 [case k=3].
```
The additional message is only created if the assert actually needs to generate output (usually when it fails).
It appears in the [] brackets at the end of the assert message.  Notice you can create fairly complex messages
by chaining things you can print (like `Serial.print()`) between `<<` operators.  This is similar to the C++ ostream insertion operators, if you are familar with that.

The status of the test can be used (bool ok) when printing the message.  Under normal verbosity settings, ok will always be false, but more verbose settings can print assert messages even if they pass.

## Selecting tests

In your setup() function, you can select which tests are going to be setup and looped.  The default is that all tests are included.

`Test::exclude(const char *pattern)` removes all the tests that match the given *-pattern.

`Test::include(const char *pattern)` includes all the tests that match the given *-pattern.

Here are some examples:

## Select examples:

A single test `my_test`

```
void setup()
{
  Serial.begin(9600);
  while(!Serial) {} // Portability for Leonardo/Micro
  Test::exclude("*");
  Test::include("my_test");
}
```

All tests named dev_-something, but not the ones ending in _skip or _slow, or have the word eeprom in them:
```
void setup()
{
  Serial.begin(9600);
  while(!Serial) {} // Portability for Leonardo/Micro
  Test::exclude("*");
  Test::include("dev_*");
  Test::exclude("*_slow");
  Test::exclude("*_skip");
  Test::exclude("*eeprom*");
}
```

# Output

The `Test::out` value is the *shared* value for all tests describing where output for all tests goes.  The default is 

```
Test::out = &Serial;
```

But you can set to the address of any Print stream. For example, if you want the output to be on the `Serial3` device on the arduino mega, use

```
Test::out = &Serial3;
```

in your `setup()`.  Note the library does not set the baud rate - you have to do that in your `setup()`.

## Verbosity

Normal ArduinoUnit verbosity reports only failed assertions, the status (pass,skip,fail) of completed tests, and a summary.

### Seeing more.

It is often useful to see the results of assertions [and footnotes] even when they pass. If you want to trace everything in this way, you can turn on all output with `Test::min_verbosity = TEST_VERBOSITY_ALL` in your setup.

### Seeing less more.

The previous choice is great until you are lost in an ocean of messages for tests you do not want to watch at the moment.  Instead of globally setting `min_verbosity/max_verbosity` in your `setup()`, you can instead use `verbosity = TEST_VERBOSITY_ALL` in a given test to see everything about that test.

## MockPrint and MockStream (intermediate)

`MockPrint` is provided by ArduinoUnit to mimic a real output device, like Serial, but is also a String which happens to contain the information printed to it.  This can be used to test output formatting, as in:
```
void format(Print &out, int value) {
  out.print("decimal ");
  out.print(value);
  out.print(" is hex ");
  out.println(value,HEX);
}

test(format) {
  MockPrint mp;
  format(mp,32); // test as mock
  assertEqual(mp,"decimal 32 is hex 20\r\n");
}

void setup() {
  Serial.begin();
  while (!Serial) {}
  format(Serial,100); // format to serial
}

void loop() {
  Test::run();
}
```

`MockStream` is provided by ArduinoUnit to mimic a real input/output device, like Serial.  It contains two `MockPrint` parts, `input` contains the input that will be read from the MockStream, and `output` which contains the output that was written.  This can be used to test input and output, as in:
```
void square(Stream &io) {
  io.print("value? ");
  int x = io.parseInt();
  out.print(value);  
  out.print("*");
  out.print(value);
  out.print("=");
  out.println(x*x);
}

test(square) {
  MockStream ms;
  ms.input.print(10);
  square(ms);
  assertEqual(ms.output,"value? 10*10=100\r\n");
}

void setup() {
  Serial.begin();
  while (!Serial) {}
  square(Serial); // format to serial
}

void loop() {
  Test::run();
}
```
The `mockstream` example shows a convenient way to switch between real and mock streams for testing.

## En Vitro Testing (advanced)

ArduinoUnit will compile in a standard C++ environment (LLVM or GCC) with -std=gnu++11.  The advanced example has a makefile and main.cpp to support this.

Note ArduinoUnit has very limited mocking features; you will have to include the mocking features you need to simulate the embedded environment.  The main.cpp file in the advanced example illustrates minimal mocking.  In particular the only features provided (because of dependencies on these by ArduinoUnit) are:
```
F()
millis()
micros()
String
Print
Printable
Stream # public components only
```
These are available via `#include "ArduinoUnitMock.h"`.  In the mock environment, there are two additional objects, `CppStreamPrint` and `CppIOStream`, which wrap C++ `std::ostream` (and `std::istream` for `CppIOStream`).  This simplifies creating tests in the mocking environments.  Look at the advanced example and test firmware for guidance.

## Verbosity (advanced)

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

## Built-in Assertions (details)

The following assertions are supported

```
assertLess(arg1,arg2)
assertLessOrEqual(arg1,arg2)
assertEqual(arg1,arg2)
assertNotEqual(arg1,arg2)
assertMoreOrEqual(arg1,arg2)
assertMore(arg1,arg2)
```
Anything that can be compared via a '<' comparison between them can be used.

All the string-like types (String, char *, char[] and flash string literals) can be used interchangeably in assertions, i.e.:
```
test(strings) {
   const char *cOk="ok";
   char aOk[3]; 
   String sOk(cOk);
   // two underbars (_) for a flash string literal...
   const __FlashStringHelper *fOk = F("ok"); 

   strcpy(aOk,cOk);

   assertEqual(cOk,aOk,F("char* vs char[]"));
   assertEqual(aOk,sOk,F("char[] vs String"));
   assertEqual(sOk,fOk,F("String vs flash"));
   assertEqual(fOk,cOk,F("flash vs char*"));   
}
```

### __FlashStringHelper ?
Note that using a flash string literal directly (except the footnote) in an assert is not supported.  You must declare and use them separately as above.  The main reason for this is the F() macro expands to a large useless expression, which is then represented in flash as part of the assert message.  The alternate version keeps the assert message small and readable.

There are addtionally some boolean assertions:
```
assertTrue(arg)
assertFalse(arg)
```
These are shorthands for `assertEqual(arg,true)` and `assertEqual(arg,false)`.

See the section below for assertions on tests.

The output from these assertions is to print a string represenation of the
arguments, and the value of the arguments, as in:
```
Assertion passed/failed: (arg1=value1) op (arg2=value2), file name, line #.
```
These assertions are defined in a way that the problem of multiple
evaluations is avoided.  The arguments are only evaluated once in these
assertions.  

All the assert macros expand to a test that creates an optional message, and, if false, calls fail() on the current test and returns.

## Meta Assertions (Advanced)

You can make assertions on the outcome of tests as well.  The following meta-assertions are supported:

| Meta Assertion | Description |
| --- | --- |
| `assertTestDone(test [,footnote[,retval]])` | test done (skip, pass or fail)?|
| `assertTestNotDone(test [,footnote[,retval]])` | test not done?|
| `assertTestPass(test [,footnote[,retval]])` | test passed? |
| `assertTestNotPass(test [,footnote[,retval]])` | test not passed (fail, skip, or not done)? |
| `assertTestFail(test [,footnote[,retval]])` | test failed? |
| `assertTestNotFail(test [,footnote[,retval]])` | test not failed (pass, skip, or not done)? |
| `assertTestSkip(test [,footnote[,retval]])` | test skipped? |
| `assertTestNotSkip(test [,footnote[,retval]])` | test not skipped (pass, fail, or not done)? |

| Meta Assertion | Description |
| --- | --- |
| `assertCurrentTestDone([footnote[,retval]])`| current test done (skip, pass or fail)?|
| `assertCurrentTestNotDone([footnote[,retval]])` | current test not done?|
| `assertCurrentTestPass([footnote[,retval]])` | current test passed? |
| `assertCurrentTestNotPass([footnote[,retval]])` | current test not passed (fail, skip, or not done)? |
| `assertCurrentTestFail([footnote[,retval]])` | current test failed? |
| `assertCurrentTestNotFail([footnote[,retval]])` | current test not failed (pass, skip, or not done)? |
| `assertCurrentTestSkip([footnote[,retval]])` | current test skipped? |
| `assertCurrentTestNotSkip([footnote[,retval]])` | current test not skipped (pass, fail, or not done)? |

These can be used in conjunction with the boolean check-only macros
```
checkTestDone(test) / checkCurrentTestDone()
checkTestNotDone(test) / checkCurrentTestNotDone()
checkTestPass(test) / checkCurrentTestPass()
checkTestNotPass(test) / checkCurrentTestNotPass()
checkTestFail(test) / checkCurrentTestFail()
checkTestNotFail(test) / checkCurrentTestNotFail()
checkTestSkip(test) / checkCurrentTestSkip()
checkTestNotSkip(test) / checkCurrentTestNotSkip()
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

## `Test` and `TestOnce` (advanced)
You can create your own modular tests by deriving from these classes.

```
class MyTest : public Test {
private:
  void construct() {
    // TODO: construct named test.
    // This should be lightweight - it may be excluded
    //
    // You can set verbosity.
  }
public:
  MyTest(const char *name) : Test(name) {construct();}
#if defined(F)
  MyTest(const __FlashStringHelper *name) {construct();}
#endif
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
  MyTestOnce(const char *name) : TestOnce(name) {
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

MyTestOnce myTestOnce1("myTestOnce1");
MyTestOnce myTestOnce2("myTestOnce2");
```

Note that `Test::run()` only calls the active unresolved tests.

## Known Bugs

* The `assertCurrentTestXXXX([,footnote [,retval])` macros do not compile on ESP8266 boards with no footnote.  Use an empty footnote `assertCurrentTestXXXX("")`, or use `assertCurrentTestXXXX_0()` for no footnote.  You do not have to specify a return value.

## FAQ

Q. The line number of the asserts do not match the source file.

A.  As far as I can tell, this is a bug in the compiler -- look two
   lines up.  I do not know why the `__LINE__` macro does not match
   the actual line of code.

Q. What's with the `# 2 "file.ino"` business in the examples?

A. This is to address question 1 above, and, without this line, the filename
   will be a very long and mostly useless name in the asserts, like,

<pre>
/var/folders/gr/n9s7qtcs2qqbdnmcgm6gjzrm0000gp/T/build2118014134542174575.tmp/sketch_mar17a.ino
</pre>

  This uses up flash memory space and doesn't give any useful information when
  something goes wrong.

Q. I get these link errors about multiply defined test_XXXX_instance.

A. You have defined two tests with the same name XXXX using either the
   test() or testing() macro.

Q. I get no output

A. Here is a troubleshooting guideline:

 * Make sure you call `Serial.begin()` in your setup.  Or, if you redirect
   output by changing the value of `Test::out`, make sure you configure
   the Print stream you direct it to.
 * If you are using an Arduino Leonardo/Micro: don't forget to add 
   `while(!Serial) {}` after `Serial.begin(9600)` in the setup(). Without this line
   nothing will be printed in the serial monitor.
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
   * Setting Test::min_verbosity = TEST_VERBOSITY_ALL (the default is TEST_VERBOSITY_TESTS_ALL | TEST_VERBOSITY_ASSERTIONS_FAILED, generating output only for failed assertions, completions of tests, and an overall summary).
   * With these settings, the per-test verbosity has no effect.

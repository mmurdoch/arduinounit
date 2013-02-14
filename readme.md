#Getting Started

Create a directory called ArduinoUnit under `<arduino installation directory>\libraries`.

Copy everything from the `src` directory to this new directory.

Open a sketch in arduino and click `Sketch | Import Library... | ArduinoUnit` to start unit testing.

For example, try this simple unit testing sketch:

    #include <ArduinoUnit.h>

    // Create test suite
    TestSuite suite;

    void setup() {
      Serial.begin(9600);
    }

    // Create a test called 'addition' in the test suite
    test(addition) {
      assertEquals(3, 1 + 2);
    }

    void loop() {
      // Run test suite, printing results to the serial port
      suite.run();
    }

Upload this sketch to the Arduino (using the 'Upload' button, `File | Upload` or `Ctrl+U`).

Turn on the Serial Monitor (using the 'Serial Monitor' button, `Tools | Serial Monitor` or 
  `Ctrl+Shift+M`) and expect to see the following:

    Running test suite...
    Tests run: 1 Successful: 1 Failed: 0

#FAQ

1. Q. I'm getting an error message like:
        `1: error: pasting "test_" and ""something"" does not give a valid preprocessing token`
        when I 'Verify' or 'Upload to I/O Board' my sketch. How can I fix this?

   A. You have declared a test like this with the name in quotes:

          test("something") {
              // ...
          }

      The name of the test must not be quoted. i.e. it should look like this:

          test(something) {
              // ...
          }

2. Q. I'm getting an error message like:
        `error: redefinition of 'SuiteAppender test_a_appender' In function 'void test_a(Test&)':`
        when I 'Verify' or 'Upload to I/O Board' my sketch. How can I fix this?

   A. You have two or more tests declared with the same name. For example:

          test(a) {
              // ...
          }
          
          test(a) {
              // ...
          }

      The names of all tests in the same sketch must be unique. This is true even if they are in 
         different test suites.

3. Q. I'm trying to write a custom assertion function but I keep getting an error message like:
        `error: '__test__' was not declared in this scope`
        when I 'Verify' or 'Upload to I/O Board' my sketch. How can I fix this?

   A. You have probably defined a custom assertion function such as the following:

          void assertStringsEqual(const char* expected, const char* actual) {
              assertEquals(strlen(expected), strlen(actual));
              for (int i = 0; i < strlen(expected); i++) {
                  assertEquals(expected[i], actual[i]);
              }
          }
          
      and are trying to use it like this from within a test function:
       
          assertStringsEqual("expected string", actualString);
          
      To fix this you need to add a `Test& __test__` parameter to the signature of the custom 
      assertion function, for example:
      
          void assertStringsEqual(Test& __test__, const char* expected, const char* actual) {
              assertEquals(strlen(expected), strlen(actual));
              for (int i = 0; i < strlen(expected); i++) {
                  assertEquals(expected[i], actual[i]);
              }
          }

      and add `__test__` as the relevant argument when you call it, for example:
      
          assertStringsEqual(__test__, "expected string", actualString);

4. Q. I'm trying to use a custom assertion function but I keep getting an error message like:
        `error: '<custom_assertion_function>' was not declared in this scope`
        when I 'Verify' or 'Upload to I/O Board' my sketch. How can I fix this?

   A. You need to define the custom assertion function above where it is being used or 
        forward-declare it, for example:
        
          // Forward declare custom assertion
          void assertStringsEqual(Test& __test__, const char* expected, const char* actual);

          // ...
          
          test(something) {
              // Use custom assertion
              assertStringsEqual(__test__, "expected string", actualString);
          }
          
          // ...

          // Define custom assertion
          void assertStringsEqual(Test& __test__, const char* expected, const char* actual) {
              assertEquals(strlen(expected), strlen(actual));
              for (int i = 0; i < strlen(expected); i++) {
                  assertEquals(expected[i], actual[i]);
              }
          }

# Version History

## Version 1.5 (3 January 2013)
### Bugs fixed:
  * Issue 4 - Added release notes for version 1.4.2
  * Issue 5 - SerialReporter fails for baud values above 9600.
    Note that the fix for this is a *breaking change* and requires
    all arduinounit sketches using the default serial reporting to
    have `Serial.begin(<baud_rate>)` added in their `setup()` function
  * Issue 6 - Line numbers are being incorrectly reported for failing assertions on Arduino 1.0.3
  * Issue 7 - ArduinoUnit unit tests print garbage output indefinitely

## Version 1.4.2 (2 October 2011)
### Features added:
Support for Arduino 1.0 environment (check for arduino version to include correct header)

## Version 1.4.1 (13 June 2011)
### Bugs fixed:
* Issue 2 - Compilation error on Arduino 0022
* Issue 3 - Incomplete MIT license terms

## Version 1.4 (28 June 2009)
### Features added:
* Checks and reporting on heap allocation failures.
* Example of using ArduinoUnit to check the correct functioning of digital I/O pins 2-13.

### Bugs fixed:
* Each failed assertion counts as a single test function failure. Two failed assertions
  in the same test function double-count the failure. Since the usual case is that 
  failing assertions short-cut a test function execution by returning this is only
  noticeable where a test function calls other functions which perform the assertions.

* Stack/heap collisions are not reported and cause incorrect behaviour, reporting (for 
  example):
  
        Equality assertion failed in 'nameReported' on line 310: expected 
        '9599460892leFailingAssertion' but was '0'

## Version 1.3 (01 June 2009)
### Features added:
* Allowed the use of variables named `test` in test functions. The name `__test__` is now a 
  reserved word and if used as a variable name within a test function e.g. like this:

        test(name) {
          int __test__ = 0;
        }    

  verifying or uploading the sketch will cause the following compilation error:
  `error: declaration of 'int __test__' shadows a parameter`

* Added keyword highlighting for `TestSuite`, `run`, `test`, `assertTrue`, `assertEquals`
  and `__test__`.

### Bugs fixed:
* Line numbers of failing boolean assertions (assertTrue()) are 3 greater than the real
  line numbers (line numbers reported for failing equality assertions are correct)

## Version 1.2 (29 May 2009)
### Features added:
* Reduced the compiled size of sketches using ArduinoUnit:

        Usage                      From (bytes)    To (bytes)    Reduction (bytes)
        -----------------------    ------------    ----------    -----------------
        Including ArduinoUnit.h    2014            1906          108
        Creating a test suite       766             774           -8
        Running a test suite        132             120           12
        -----------------------    ------------    ----------    -----------------
        Total                      2912            2800          112

### Bugs fixed:
* Using `Sketch | Import Library | ArduinoUnit` `#includes` all the header files in the 
  ArduinoUnit directory rather than just `ArduinoUnit.h`

## Version 1.1 (27 May 2009)
### Bugs fixed:
* When using multiple test suites in a sketch all assertions are reported in against the last 
  defined suite.

## Version 1.0 (25 May 2009)
### Features added:
* Simple test definition and execution
* Boolean assertions
* Integer equality assertions
* Default reporting of results to the serial port
* Ability to customize result reporting (e.g. where serial port is being used by the code under test)
* Support for multiple (optionally named) test suites

The API changes from ArduinoUnit 1.x to 2.x are designed to simplify the writing of tests. 
To port existing tests written against ArduinoUnit 1.x to instead use ArduinoUnit 2.x use
the following guidelines.

Remove all test suite creation such as:

    TestSuite suite;

Replace all uses of `suite.run()` with `Test::run()`

Replace all uses of `assertEquals()` with `assertEqual()`

Remove all uses of `__test__`.

For example:

    // Custom assertion
    void assertStringsEqual(Test& __test__, const char* expected, const char* actual) {
        // ...
    }

    // Test using custom assertion
    test(xxx) {
        // ...
        assertStringsEqual(__test__, expected, actual);
    }
    
will become:

    // Custom assertion
    void assertStringsEqual(const char* expected, const char* actual) {
        // ...
    }
    
    // Test using custom assertion
    test(xxx) {
        // ...
        assertStringsEqual(expected, actual);
    }

**Note**: assertions on strings are now built-in, so the best port of the
above assertion would actually be to remove the redundant definition and simply use
the built-in `assertEqual` macro in ArduinoTest.  See the readme for additional
information.

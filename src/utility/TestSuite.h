/*
Copyright (c) 2009-2013 Matthew Murdoch

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
*/
#ifndef TEST_SUITE_H
#define TEST_SUITE_H

class Test;
class Reporter;

/**
 * A test suite consisting of a number of tests.
 *
 * @author Matthew Murdoch
 */
class TestSuite {
public:
    /**
     * Creates a test suite and sets it as the active suite if there is no
     * currently active suite.
     *
     * @param name the name of this test suite (optional, defaults to empty string)
     */
    TestSuite(const char* name = "");

    /**
     * Destroys this test suite.
     */
    ~TestSuite();

    /**
     * Returns whether or not there is an active suite.
     *
     * @return true if there is an active suite, false otherwise
     */
    static bool isActiveSuite();

    /**
     * Returns the active test suite. If called when isActiveSuite() is false
     * the results are undefined.
     *
     * @return the active test suite
     */
    static TestSuite& getActiveSuite();

    /**
     * Sets the active test suite.
     *
     * @param suite active test suite to set
     */
    static void setActiveSuite(TestSuite& suite);

    /**
     * Returns the name of this test suite.
     *
     * @return test suite name
     */
    const char* getName() const;

    /**
     * Returns the reporter which reports on the outcome of this suite.
     *
     * @return current reporter
     */
    Reporter& getReporter() const;

    /**
     * Sets the reporter to report on the outcome of this suite.
     *
     * @param reporter reporter to set
     */
    void setReporter(Reporter& reporter);

    /**
     * Adds a test to this suite.
     *
     * @param test test to add
     */
    void add(Test& test);

    /**
     * Runs this test suite. If this suite has already run calling
     * this function has no effect.
     *
     * @return true if the run was successful, false if an internal error occurred
     */
    bool run();

    /**
     * Returns whether or not this suite has finished running.
     *
     * @return true if this suite has finished running, false otherwise
     */
    bool hasCompleted() const;

    /**
     * Returns the number of test functions in this suite.
     *
     * @return the number of test functions in this suite
     */
    int getTestCount() const;

    /**
     * Returns the number of successful test functions in this suite.
     *
     * @return the number of successful test functions in this suite
     */
    int getSuccessCount() const;

    /**
     * Returns the number of failed test functions in this suite.
     *
     * @return the number of failed test functions in this suite
     */
    int getFailureCount() const;

    /**
     * Asserts that a condition is true.
     *
     * @param test test in which assertion occurred
     * @param condition condition to check
     * @param lineNumber line number on which assertion lives
     */
    void suiteAssertTrue(Test& test, bool condition, int lineNumber);

    /**
     * Asserts that two values are equal.
     *
     * @param test test in which assertion occurred
     * @param first first value to test for equality
     * @param second second value to test for equality
     * @param lineNumber line number on which assertion lives
     */
    void suiteAssertEquals(Test& test, int first, int second, int lineNumber);

private:
    /**
     * Adds a failure to this test suite.
     */
    void addFailure();

    static TestSuite* activeSuite;

    const char* name;
    Test* head;
    int successCount;
    int failureCount;
    bool completed;
    Reporter* reporter;
};

#endif // TEST_SUITE_H

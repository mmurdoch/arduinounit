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
#ifndef REPORTER_H
#define REPORTER_H

class Test;
class TestSuite;

/**
 * Reports test suite outcomes.
 *
 * @author Matthew Murdoch
 */
class Reporter {
public:
    /**
     * Creates a test suite reporter.
     */
    Reporter() { }

    /**
     * Initializes this reporter.
     *
     * @param name name of test suite on which to report
     */
    virtual void begin(const char* name) = 0;

    /**
     * Reports on a test failure.
     *
     * @param test failing test
     * @param lineNumber line number on which test failed
     */
    virtual void reportFailure(const Test& test, int lineNumber) = 0;

    /**
     * Reports on an equality test failure.
     *
     * @param test failing test
     * @param lineNumber line number on which test failure
     * @param expected expected value
     * @param actual actual value
     */
    virtual void reportEqualityFailure(const Test& test, int lineNumber, const char* expected, const char* actual) = 0;

    /**
     * Reports on the outcome of a test suite.
     *
     * @param suite test suite on which to report
     */
    virtual void reportComplete(const TestSuite& suite) = 0;
};

#endif // REPORTER_H

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
#ifndef TEST_H
#define TEST_H

class TestSuite;

/**
 * A test.
 *
 * @author Matthew Murdoch
 */
class Test {
public:
    /**
     * Creates a test, appending it to a test suite.
     *
     * @param suite suite to which to append test
     * @param name test name
     * @param testFunction the body of the test
     */
    Test(TestSuite& suite_, const char* name_, void (*testFunction_)(Test&));

    TestSuite* suite;
    const char* name;

private:
    friend class TestSuite;
    void (*testFunction)(Test&);
    bool successful;
    Test* next;
};

#endif // TEST_H

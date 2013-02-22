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
#ifndef ARDUINO_UNIT_H
#define ARDUINO_UNIT_H

#include "utility/Test.h"
#include "utility/TestSuite.h"
#include "utility/SuiteAppender.h"
#include "utility/FakeStream.h"

/**
 * Defines a test function in the active test suite.
 *
 * @param name name of test function to define
 */
#define test(name)\
    testInSuite(name, TestSuite::getActiveSuite())

/**
 * Defines a test function.
 *
 * @param name name of test function to define
 * @param suite test suite in which to define test function
 */
#define testInSuite(name, suite)\
    void test_##name(Test&);\
    struct Test_##name : Test {\
        Test_##name(TestSuite& st, const char* nm, void (*testFunction)(Test&)) : Test(st, nm, testFunction) {}\
    } test_##name##_instance(suite, #name, test_##name);\
    SuiteAppender test_##name##_appender(suite, test_##name##_instance);\
    void test_##name(Test& __test__)

/**
 * Asserts that a condition is true in the active test suite.
 * If the condition is false the test function ends prematurely
 * and further assertions are not checked.
 *
 * @param condition boolean condition to check
 */
#define assertTrue(condition)\
    __test__.suite->suiteAssertTrue(__test__, (condition), __LINE__);\
    if (!(condition)) {\
        return;\
    }

/**
 * Asserts that two values are equal in the active test suite.
 * If the values differ the test function ends prematurely
 * and further assertions are not checked.
 *
 * @param expected expected value
 * @param actual actual value
 */
#define assertEquals(expected, actual)\
    __test__.suite->suiteAssertEquals(__test__, (expected), (actual), __LINE__);\
    if ((expected) != (actual)) {\
        return;\
    }

#endif // ARDUINO_UNIT_H

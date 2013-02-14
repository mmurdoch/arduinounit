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
#include "TestSuite.h"

#include "Test.h"
#include "SerialReporter.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct TestLink {
    Test test;
    TestLink* next;
};

TestSuite* TestSuite::activeSuite = NULL;

TestSuite::TestSuite(const char* suiteName) :
    name(suiteName), head(NULL), successCount(0), failureCount(0), completed(false), error(false) {

    setReporter(serialReporter);

    TestSuite::setActiveSuite(*this);
}

TestSuite::~TestSuite() {
    TestLink* current = head;
    while (current != NULL) {
        TestLink* next = current->next;
        free(current);
        current = next;
    }
}

bool TestSuite::isActiveSuite() {
    return TestSuite::activeSuite != NULL;
}

TestSuite& TestSuite::getActiveSuite() {
    return *(TestSuite::activeSuite);
}

void TestSuite::setActiveSuite(TestSuite& suite) {
    TestSuite::activeSuite = &suite;
}

const char* TestSuite::getName() const {
    return name;
}

Reporter& TestSuite::getReporter() const {
    return *reporter;
}

void TestSuite::setReporter(Reporter& reporter_) {
    reporter = &reporter_;
}

void TestSuite::add(const char* name, void (*testFunction)(Test&)) {
    TestLink* newLink = (TestLink*) malloc(sizeof(TestLink));
    if (newLink != NULL) {
        newLink->test.suite = this;
        newLink->test.testFunction = testFunction;
        newLink->test.name = name;
        // Default to true so that a test with no assertions doesn't cause failure
        newLink->test.successful = true;
        newLink->next = NULL;

        TestLink** newTail;
        if (head == NULL) {
            newTail = &head;
        } else {
            TestLink* tail = head;
            while (tail->next != NULL) {
                tail = tail->next;
            }
            newTail = &(tail->next);
        }

        *newTail = newLink;
        successCount++;
    } else {
        error = true;
    }
}

int TestSuite::getTestCount() const {
    return successCount + failureCount;
}

int TestSuite::getFailureCount() const {
    return failureCount;
}

int TestSuite::getSuccessCount() const {
    return successCount;
}

bool TestSuite::run() {
    if (error) {
        return false;
    }

    if (!completed) {
        reporter->begin(name);

        TestLink* current = head;
        while (current != NULL) {
            current->test.testFunction(current->test);
            current = current->next;
        }

        completed = true;

        reporter->reportComplete(*this);
    }

    return true;
}
bool TestSuite::hasCompleted() const {
    return completed;
}

void TestSuite::suiteAssertTrue(Test& test, bool condition, int lineNumber) {
    if (!condition && test.successful) {
        test.successful = false;
        addFailure();
        reporter->reportFailure(test, lineNumber);
    }
}

/**
 * The maximum length (sign and digits) of a decimal integer.
 */
#define MAX_INTEGER_LENGTH 11

/**
 * Converts an integer to a string.
 *
 * @param integer integer to convert
 * @param string character buffer to fill
 */
#define integerToString(integer, string) itoa((integer), (string), 10)

void TestSuite::suiteAssertEquals(Test& test, int expected, int actual, int lineNumber) {
    bool areEqual = (expected == actual);
    if (!areEqual && test.successful) {
        test.successful = false;
        addFailure();

        char expectedBuffer[MAX_INTEGER_LENGTH];
        integerToString(expected, expectedBuffer);

        char actualBuffer[MAX_INTEGER_LENGTH];
        integerToString(actual, actualBuffer);

        reporter->reportEqualityFailure(test, lineNumber, expectedBuffer, actualBuffer);
    }
}

void TestSuite::addFailure() {
    successCount--;
    failureCount++;
}

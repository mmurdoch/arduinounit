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
/**
 * The main unit tests for ArduinoUnit.
 *
 * @author Matthew Murdoch
 */
#include <ArduinoUnit.h>
#include <utility/Reporter.h>

void assertStringsEqual(Test& __test__, const char* expected, const char* actual);

class NonReportingReporter : public Reporter {
public:
    void begin(const char* /*name*/) { }
    void reportFailure(const Test& /*test*/, int /*lineNumber*/) { }
    void reportEqualityFailure(const Test& /*test*/, int /*lineNumber*/, const char* /*expected*/, const char* /*actual*/) { }
    void reportComplete(const TestSuite& /*suite*/) { }
};

NonReportingReporter nonReportingReporter;

TestSuite suite("main");

void setup() {
    Serial.begin(9600);
}

void loop() {
    suite.run();
}

void runSuite(TestSuite& aSuite) {
    aSuite.setReporter(nonReportingReporter);
    aSuite.run();
}

TestSuite empty;

testInSuite(emptySuite, suite) {
    runSuite(empty);

    assertEquals(0, empty.getTestCount());
    assertEquals(0, empty.getSuccessCount());
    assertEquals(0, empty.getFailureCount());
}

TestSuite noAssertions;

test(noAssertions) {
}

testInSuite(singleTestNoAssertions, suite) {
    runSuite(noAssertions);

    assertEquals(1, noAssertions.getTestCount());
    assertEquals(1, noAssertions.getSuccessCount());
    assertEquals(0, noAssertions.getFailureCount());
}

TestSuite singleSuccessfulAssertion;

test(singleSuccessful) {
    assertTrue(true);
}

testInSuite(singleSuccessfulAssertion, suite) {
    runSuite(singleSuccessfulAssertion);

    assertEquals(1, singleSuccessfulAssertion.getTestCount());
    assertEquals(1, singleSuccessfulAssertion.getSuccessCount());
    assertEquals(0, singleSuccessfulAssertion.getFailureCount());
}

TestSuite singleFailingAssertion;

test(singleFailing) {
    assertTrue(false);
}

testInSuite(singleFailingAssertion, suite) {
    runSuite(singleFailingAssertion);

    assertEquals(1, singleFailingAssertion.getTestCount());
    assertEquals(0, singleFailingAssertion.getSuccessCount());
    assertEquals(1, singleFailingAssertion.getFailureCount());
}

TestSuite failBeforeSuccessFails;

test(failBeforeSuccess) {
    assertTrue(false);
    assertTrue(true);
}

testInSuite(failBeforeSuccessFails, suite) {
    runSuite(failBeforeSuccessFails);

    assertEquals(1, failBeforeSuccessFails.getTestCount());
    assertEquals(0, failBeforeSuccessFails.getSuccessCount());
    assertEquals(1, failBeforeSuccessFails.getFailureCount());
}

TestSuite singleSuccessfulEqualityAssertion;

test(singleSuccessfulEquality) {
    assertEquals(15, 15);
}

testInSuite(singleSuccessfulEqualityAssertion, suite) {
    runSuite(singleSuccessfulEqualityAssertion);

    assertEquals(1, singleSuccessfulEqualityAssertion.getTestCount());
    assertEquals(1, singleSuccessfulEqualityAssertion.getSuccessCount());
    assertEquals(0, singleSuccessfulEqualityAssertion.getFailureCount());
}

TestSuite singleFailingEqualityAssertion;

test(singleFailingEquality) {
    assertEquals(0, 1);
}

testInSuite(singleFailingEqualityAssertion, suite) {
    runSuite(singleFailingEqualityAssertion);

    assertEquals(1, singleFailingEqualityAssertion.getTestCount());
    assertEquals(0, singleFailingEqualityAssertion.getSuccessCount());
    assertEquals(1, singleFailingEqualityAssertion.getFailureCount());
}

TestSuite equalityFailBeforeSuccessFails;

test(equalityFailBeforeSuccess) {
    assertEquals(87, 88);
    assertEquals(16, 16);
}

testInSuite(equalityFailBeforeSuccessFails, suite) {
    runSuite(equalityFailBeforeSuccessFails);

    assertEquals(1, equalityFailBeforeSuccessFails.getTestCount());
    assertEquals(0, equalityFailBeforeSuccessFails.getSuccessCount());
    assertEquals(1, equalityFailBeforeSuccessFails.getFailureCount());
}

TestSuite twoTestsSuite;

test(testOne) {
}

test(testTwo) {
}

testInSuite(twoTestsSuite, suite) {
    runSuite(twoTestsSuite);

    assertEquals(2, twoTestsSuite.getTestCount());
    assertEquals(2, twoTestsSuite.getSuccessCount());
    assertEquals(0, twoTestsSuite.getFailureCount());
}

TestSuite twoTestsOneFailingOneSucceeding;

test(testOneFailing) {
    assertEquals(1, 5);
}

test(testTwoSucceeding) {
    assertEquals(44, 44);
}

testInSuite(twoTestsOneFailingOneSucceeding, suite) {
    runSuite(twoTestsOneFailingOneSucceeding);

    assertEquals(2, twoTestsOneFailingOneSucceeding.getTestCount());
    assertEquals(1, twoTestsOneFailingOneSucceeding.getSuccessCount());
    assertEquals(1, twoTestsOneFailingOneSucceeding.getFailureCount());
}

// Failure manifests as compilation error: declaration of 'int test' shadows a parameter
testInSuite(variableNamedTestOk, suite) {
    int test = 0;
}

testInSuite(variableNamedSuiteOk, suite) {
    int suite = 0;
}

class ReporterSpy : public Reporter {
public:
    ReporterSpy() : suiteName("unitialized") { } 
    void begin(const char* name) { suiteName = name; }
    void reportFailure(const Test& test, int lineNumber) { failedTest = &test; failedLineNumber = lineNumber; }
    void reportEqualityFailure(const Test& test, int lineNumber, const char* expected, const char* actual) { 
        equalityFailedTest = &test; equalityFailedLineNumber = lineNumber; equalityFailedExpected = expected; equalityFailedActual = actual; }
    void reportComplete(const TestSuite& suite) { completeSuite = &suite; }
    
    const char* suiteName;
    const Test* failedTest;
    int failedLineNumber;
    const Test* equalityFailedTest;
    int equalityFailedLineNumber;
    const char* equalityFailedExpected;
    const char* equalityFailedActual;
    const TestSuite* completeSuite;
};

testInSuite(noNameReported, suite) {
    TestSuite noName;
    ReporterSpy reporterSpy;
    noName.setReporter(reporterSpy);

    assertTrue(noName.run());

    assertStringsEqual(__test__, "", reporterSpy.suiteName);
}

testInSuite(nameReported, suite) {
    const char* name = "name";
    TestSuite named(name);
    ReporterSpy reporterSpy;
    named.setReporter(reporterSpy);

    assertTrue(named.run());

    assertStringsEqual(__test__, name, reporterSpy.suiteName);
}

TestSuite failureReported;

test(failureReportedTest) {
    assertTrue(false); // Line 252 (changing this will break the following test)
}

testInSuite(failuredReported, suite) {
    ReporterSpy reporterSpy;
    failureReported.setReporter(reporterSpy);

    failureReported.run();
    
    assertStringsEqual(__test__, "failureReportedTest", reporterSpy.failedTest->name);
    assertEquals(252, reporterSpy.failedLineNumber);
}

TestSuite equalityFailureReported;

test(equalityFailureReportedTest) {
    assertEquals(17, 63); // Line 268 (changing this will break the following test)
}

testInSuite(equalityFailureReported, suite) {
    ReporterSpy reporterSpy;
    equalityFailureReported.setReporter(reporterSpy);
    
    equalityFailureReported.run();

    assertStringsEqual(__test__, "equalityFailureReportedTest", reporterSpy.equalityFailedTest->name);
    assertEquals(268, reporterSpy.equalityFailedLineNumber);
    assertStringsEqual(__test__, "17", reporterSpy.equalityFailedExpected);
    assertStringsEqual(__test__, "63", reporterSpy.equalityFailedActual);
}

TestSuite completeReported("completeReportedName");

testInSuite(completeReported, suite) {
    ReporterSpy reporterSpy;
    completeReported.setReporter(reporterSpy);
    
    completeReported.run();
    
    assertStringsEqual(__test__, "completeReportedName", reporterSpy.completeSuite->getName());
}

TestSuite nestedAssertionFailures;

void fail(Test& __test__) {
    assertTrue(false);
}

test(nestedAssertionFailuresTest) {
    fail(__test__);
    fail(__test__);
}

testInSuite(nestedAssertionFailures, suite) {
    ReporterSpy reporterSpy;
    nestedAssertionFailures.setReporter(reporterSpy);
    
    nestedAssertionFailures.run();
    
    assertEquals(1, reporterSpy.completeSuite->getFailureCount());
}

TestSuite nestedEqualityAssertionFailures;

void failEquality(Test& __test__) {
    assertEquals(1, 2);
}

test(nestedEqualityAssertionFailuresTest) {
    failEquality(__test__);
    failEquality(__test__);
}

testInSuite(nestedEqualityAssertionFailures, suite) {
    ReporterSpy reporterSpy;
    nestedEqualityAssertionFailures.setReporter(reporterSpy);
    
    nestedEqualityAssertionFailures.run();
    
    assertEquals(1, reporterSpy.completeSuite->getFailureCount());
}

void assertStringsEqual(Test& __test__, const char* expected, const char* actual) {
    assertEquals(strlen(expected), strlen(actual));
    for (size_t i = 0; i < strlen(expected); i++) {
        assertEquals(expected[i], actual[i]);
    }
}


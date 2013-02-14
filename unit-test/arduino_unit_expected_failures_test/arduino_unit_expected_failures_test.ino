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
 * These tests are all expected to fail. This sketch has been added to validate
 * that assertions fail when they are supposed to. To ensure that they are
 * correctly failing validation of these tests must be done *without* using
 * ArduinoUnit.
 *
 * @author Matthew Murdoch
 */
#include <ArduinoUnit.h>

TestSuite suite;

void setup() {
    Serial.begin(9600);
}

test(failingAssertion) {
    assertTrue(false);
}

test(failingEqualityAssertion) {
    assertEquals(0, 1);
}

test(failingAssertionAfterSuccess) {
    assertTrue(true);
    assertTrue(false);
}

test(failingEqualityAssertionAfterSuccess) {
    assertEquals(0, 0);
    assertEquals(0, 1);
}

test(twoFailingAssertions) {
    assertTrue(false);
    assertTrue(false);
}

test(twoFailingEqualityAssertions) {
    assertEquals(0, 1);
    assertEquals(0, 1);
}

test(failingAssertionAfterSuccessfulEqualityAssertion) {
    assertEquals(0, 0);
    assertTrue(false);
}

test(failingEqualityAssertionAfterSuccessfulAssertion) {
    assertTrue(true);
    assertEquals(0, 1);
}

void loop() {
    suite.run();
}

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
 * This ArduinoUnit example demonstrates how to check for the correct functioning 
 * of digital pins 2 to 13. For the tests to pass the following pins must be connected 
 * (e.g. by using 0.1 inch jump wires):
 *
 * - pin 2 to pin 3
 * - pin 4 to pin 5
 * - pin 6 to pin 7
 * - pin 8 to pin 9
 * - pin 10 to pin 11
 * - pin 12 to pin 13
 *
 * Note pins 0 and 1 are used for the serial communication of the default test reporter
 * so cannot be tested in this way without using a custom reporter.
 *
 * @author Matthew Murdoch
 */
#include <ArduinoUnit.h>

TestSuite suite;

void setup() {
    Serial.begin(9600);
}

/**
 * Tests writing a value from a digital output pin to a digital input pin.
 * It tests two cycles of a HIGH then a LOW level to ensure all transitions
 * (HIGH to LOW and LOW to HIGH are covered).
 */
void testOutputToInput(Test& __test__, int out, int in) {
    pinMode(out, OUTPUT);
    pinMode(in, INPUT);

    int cycleCount = 2;
    int cycleLength = 2;
    int cycle[] = { HIGH, LOW };

    for (int i = 0; i < cycleCount; i++) {
        for (int j = 0; j < cycleLength; j++) {
            digitalWrite(out, cycle[j]);
            assertEquals(cycle[j], digitalRead(in));
        }
    }
}

/**
 * Tests two connected digital pins, first with one as an output and the other
 * as an input, then vice versa.
 */
void testConnectedPins(Test& __test__, int pinOne, int pinTwo) {
    testOutputToInput(__test__, pinOne, pinTwo);
    testOutputToInput(__test__, pinTwo, pinOne);    
}

test(pins2And3) {
    testConnectedPins(__test__, 2, 3);
}

test(pins4And5) {
    testConnectedPins(__test__, 4, 5);
}

test(pins6And7) {
    testConnectedPins(__test__, 6, 7);
}

test(pins8And9) {
    testConnectedPins(__test__, 8, 9);
}

test(pins10And11) {
    testConnectedPins(__test__, 10, 11);
}

test(pins12And13) {
    testConnectedPins(__test__, 12, 13);
}

void loop() {
    suite.run();
}

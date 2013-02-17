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
#include "SerialReporter.h"

#include "Test.h"
#include "TestSuite.h"
#include "ArduinoCompatibility.h"

#include <HardwareSerial.h>

SerialReporter serialReporter;

SerialReporter::SerialReporter(int baud) : baudRate(baud) {
}

void SerialReporter::begin(const char* name) {
    Serial.print("Running test suite");
    if (name[0] != 0) {
        Serial.print(" '");
        Serial.print(name);
        Serial.print("'");
    }
    Serial.println("...");
}

void SerialReporter::reportFailure(const Test& test, int lineNumber) {
    Serial.print("Assertion failed in '");
    Serial.print(test.name);
    Serial.print("' on line ");
    Serial.println(lineNumber);
}

void SerialReporter::reportEqualityFailure(const Test& test, int lineNumber, const char* expected, const char* actual) {
    Serial.print("Equality assertion failed in '");
    Serial.print(test.name);
    Serial.print("' on line ");
    Serial.print(lineNumber);
    Serial.print(": expected '");
    Serial.print(expected);
    Serial.print("' but was '");
    Serial.print(actual);
    Serial.println("'");
}

void SerialReporter::reportComplete(const TestSuite& suite) {
    Serial.print("Tests run: ");
    Serial.print(suite.getTestCount());
    Serial.print(" Successful: ");
    Serial.print(suite.getSuccessCount());
    Serial.print(" Failed: ");
    Serial.println(suite.getFailureCount());
}

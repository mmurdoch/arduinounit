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
#ifndef SERIAL_REPORTER_H
#define SERIAL_REPORTER_H

#include "Reporter.h"

/**
 * Reports test suite outcomes by printing messages with Serial.
 *
 * @author Matthew Murdoch
 */
class SerialReporter : public Reporter {
public:
    /**
     * Creates a serial suite test reporter.
     *
     * @param baudRate baud rate (bits per second) to use
     */
    SerialReporter(int baudRate = 9600);

    void begin(const char* name);

    void reportFailure(const Test& test, int lineNumber);

    void reportEqualityFailure(const Test& test, int lineNumber, const char* expected, const char* actual);

    void reportComplete(const TestSuite& suite);

private:
    int baudRate;
};

extern SerialReporter serialReporter;

#endif // SERIAL_REPORTER_H

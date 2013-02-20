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
 * Unit tests for FakeStream.
 *
 * @author Matthew Murdoch
 */
#include <ArduinoUnit.h>

TestSuite suite;

void setup() {
    Serial.begin(9600);
}

void loop() {
    suite.run();
}

test(fakeStreamInitializesToEndOfStream) {
    FakeStream stream;

    assertEquals(-1, stream.read());
}

test(fakeStreamReadsNextByte) {
    FakeStream stream;
    
    stream.nextByte(2);
    
    assertEquals(2, stream.read());
}

test(fakeStreamPeeksNextByte) {
    FakeStream stream;

    stream.nextByte(3);

    assertEquals(3, stream.peek());
}

test(fakeStreamRetainsByteAfterPeek) {
    FakeStream stream;
    stream.nextByte(1);

    stream.peek();

    assertEquals(1, stream.read());
}

test(fakeStreamRemovesByteAfterRead) {
    FakeStream stream;
    stream.nextByte(1);

    stream.read();
    
    assertEquals(-1, stream.read());
}


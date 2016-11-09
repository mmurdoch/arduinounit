/*
Copyright (c) 2014 Matt Paine

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <ArduinoUnit.h>

test (FakeStreamBufferConstructor) {
    FakeStreamBuffer stream = FakeStreamBuffer();
    assertEqual(0, stream.available());
    assertEqual(-1, stream.read());
}

// test (FakeStreamBufferDestructor) {}

test (FakeStreamBufferReset) {
    FakeStreamBuffer stream = FakeStreamBuffer();
    stream.nextBytes("12345");
    assertEqual(5, stream.available());
    assertEqual('1', stream.read());
    stream.reset();
    assertEqual(0, stream.available());
    assertEqual(-1, stream.read());
}

test (FakeStreamBufferSetToEndOfStream) {
    FakeStreamBuffer stream = FakeStreamBuffer();
    stream.nextBytes("ab");
    assertEqual(2, stream.available());
    stream.setToEndOfStream();
    assertEqual(2, stream.available());
    stream.nextBytes("cd");
    assertEqual(2, stream.available());

    assertEqual('a', stream.read());
    assertEqual('b', stream.read());
    assertEqual(0, stream.available());
    assertEqual(-1, stream.read());
    assertEqual(2, stream.available());
}

test (FakeStreamBufferNextByte) {
    FakeStreamBuffer stream = FakeStreamBuffer();
    stream.nextByte('a');
    assertEqual(1, stream.available());
    stream.nextByte('b');
    assertEqual(2, stream.available());
    assertEqual('a', stream.read());
    assertEqual('b', stream.read());
    assertEqual(-1, stream.read());
}

test (FakeStreamBufferNextBytes) {
    FakeStreamBuffer stream = FakeStreamBuffer();
    stream.nextBytes("12345");
    assertEqual(5, stream.available());
    assertEqual('1', stream.read());
    assertEqual('2', stream.read());
    assertEqual('3', stream.read());
    assertEqual('4', stream.read());
    assertEqual('5', stream.read());
    assertEqual(0, stream.available());
    assertEqual(-1, stream.read());
}

test (FakeStreamBufferAvailable) {
    FakeStreamBuffer stream = FakeStreamBuffer();
    stream.nextBytes("12345");
    stream.setToEndOfStream();
    stream.nextBytes("ab");
    assertEqual(5, stream.available());
    assertEqual('1', stream.read());
    assertEqual(4, stream.available());
    assertEqual('2', stream.read());
    assertEqual(3, stream.available());
    assertEqual('3', stream.read());
    assertEqual(2, stream.available());
    assertEqual('4', stream.read());
    assertEqual(1, stream.available());
    assertEqual('5', stream.read());
    assertEqual(0, stream.available());
    assertEqual(-1, stream.read());

    assertEqual(2, stream.available());
    assertEqual('a', stream.read());
    assertEqual(1, stream.available());
    assertEqual('b', stream.read());
    assertEqual(0, stream.available());
    assertEqual(-1, stream.read());
}

// test (FakeStreamBufferRead) {}

test (FakeStreamBufferPeek) {
    FakeStreamBuffer stream = FakeStreamBuffer();
    stream.nextBytes("12");
    assertEqual(2, stream.available());
    assertEqual('1', stream.peek());
    assertEqual(2, stream.available());
    stream.read();
    assertEqual(1, stream.available());
    assertEqual('2', stream.peek());
    assertEqual(1, stream.available());
    stream.read();
    assertEqual(0, stream.available());
    assertEqual(-1, stream.peek());
}

void setup() {
  Serial.begin(115200);
  Serial.println("");
}

void loop() {
  Test::run();
}

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

#pragma once

#include "Arduino.h"
#include "FakeStream.h"

struct BufferNode {
  byte value;
  BufferNode *next;
};

/**
 * A fake stream which can be used in place of other streams to inject bytes to
 * be read and to record any bytes written.
 *
 * @author Matthew Paine
 **/
class FakeStreamBuffer : public FakeStream {
    public:
        /**
         * Creates a fake stream.
         **/
        FakeStreamBuffer();

        /**
         * Destroys this fake stream.
         **/
        virtual ~FakeStreamBuffer();

        /**
         * Reset the FakeStream so that it can be reused across tests.  When called,
         * 'bytesWritten' becomes empty  ("") and the buffer is reset.
         **/
        void reset();

        /**
         * Pushes an end of stream (-1) to the end of the buffer
         **/
        void setToEndOfStream();

        /**
         * Appends the current buffer of values to be read via read() or peek()
         *
         * @param b the byte value
         **/
        void nextByte(byte b);

        /**
         * Appends the current buffer of values to be read via read() or peek()
         *
         * @param b the byte value
         **/
        void nextBytes(const char *s);

        /**
         * The number of bytes available to be read.
         *
         * @return the number of available bytes
         **/
        int available();

        /**
         * Reads a byte, removing it from the stream.
         *
         * @return the next byte in the buffer, or -1 (end-of-stream) if the buffer
         *         is empty or setToEndOfStream() has been called
         **/
        int read();

        /**
         * Reads a byte without removing it from the stream.
         *
         * @return the next byte in the buffer, or -1 (end-of-stream) if the buffer
         *         is empty or setToEndOfStream() has been called
         **/
        int peek();

    private:
        BufferNode *_firstNode;
        BufferNode *_lastNode;

        BufferNode* _createNode (byte value);
        void _appendNode(BufferNode *node);
        void _pushByte(byte value);
        BufferNode* _popNode();
        byte _nextByte();
        int _getBufferSize();
        int _getBufferSize(byte stopAt);
        void _clearBuffer();
};

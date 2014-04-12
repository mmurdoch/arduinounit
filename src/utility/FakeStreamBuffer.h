/*
Copyright (c) 2014, Matthew Paine, mattsoftware.com
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the mattsoftware.com nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MATTHEW PAINE BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
        void nextBytes(char *s);

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
        void _clearBuffer();
};

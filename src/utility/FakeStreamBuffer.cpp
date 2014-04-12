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

#include "FakeStreamBuffer.h"

FakeStreamBuffer::FakeStreamBuffer() : _firstNode(NULL), _lastNode(NULL) {
}

FakeStreamBuffer::~FakeStreamBuffer() {
}

void FakeStreamBuffer::reset() {
    _clearBuffer();
    FakeStream::reset();
}

void FakeStreamBuffer::setToEndOfStream() {
    _pushByte(-1);
}

void FakeStreamBuffer::nextByte(byte b) {
    _pushByte(b);
}

void FakeStreamBuffer::nextBytes(char *s) {
    int next = 0;
    while (s[next] != 0) {
        nextByte((byte)s[next]);
        next++;
    }
}

int FakeStreamBuffer::available()  {
    return _getBufferSize();
}

int FakeStreamBuffer::read() {
    if (_firstNode != NULL) {
        return _nextByte();
    }
    return -1;
}

int FakeStreamBuffer::peek() {
  if (_firstNode != 0) {
    return _firstNode->value;
  }
  return -1;
}

BufferNode* FakeStreamBuffer::_createNode (byte value) {
    BufferNode *node = (BufferNode*)malloc(sizeof(BufferNode));
    node->value = value;
    node->next = NULL;
    return node;
}

void FakeStreamBuffer::_appendNode(BufferNode *node) {
    if (_firstNode == NULL) {
        // This is the only node!
        _firstNode = node;
        _lastNode = node;
    }
    else {
        _lastNode->next = node;
        _lastNode = node;
    }
}

void FakeStreamBuffer::_pushByte (byte value) {
    BufferNode *node = _createNode (value);
    _appendNode(node);
}

BufferNode* FakeStreamBuffer::_popNode() {
    BufferNode *node = _firstNode;
    if (node != NULL) {
        _firstNode = node->next;
    }
    return node;
}

byte FakeStreamBuffer::_nextByte() {
    byte ret = -1;
    BufferNode *node = _popNode();
    if (node != NULL) {
        ret = node->value;
        free(node);
    }
    return ret;
}

int FakeStreamBuffer::_getBufferSize () {
    int count = 0;
    BufferNode *node = _firstNode;
    if (node != 0) {
        count = 1;
        while (node->next != 0) {
            node = node->next;
            count++;
        }
    }
    return count;
}

void FakeStreamBuffer::_clearBuffer() {
    BufferNode *node = _firstNode;
    BufferNode *nextNode;
    while (node != 0) {
        nextNode = node->next;
        free(node);
        node = nextNode;
    }
}

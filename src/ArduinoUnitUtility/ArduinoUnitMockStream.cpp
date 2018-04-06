#include "ArduinoUnitMockTime.h"
#include "ArduinoUnitMockStream.h"

#if !defined(ARDUINO)

Stream::Stream() : timeout(DEFAULT_TIMEOUT) {}

void Stream::setTimeout(unsigned long _timeout)
{
  timeout = _timeout;
}

unsigned long Stream::getTimeout() const { return timeout; }

bool Stream::find(char *target)
{
  return findUntil(target, strlen(target),0,0);
}

bool Stream::find(uint8_t *target)
{
  return findUntil((char*) target, strlen((char *)target),0,0);
}

bool Stream::find(char *target, size_t length)
{
  return findUntil(target,strlen(target),0,0);
}

bool Stream::findUntil(char *target, char *terminator)
{
  return findUntil(target,target != 0 ? strlen(target) : 0,terminator,terminator != 0 ? strlen(terminator) : 0);
}

bool Stream::findUntil(uint8_t *target, char *terminator)
{
  return findUntil((char *)target,target != 0 ? strlen((char *)target) : 0,terminator,terminator != 0 ? strlen(terminator) : 0);
}


bool Stream::findUntil(char *str0, size_t len0, char *str1, size_t len1)
{
  int at0 = 0;
  int at1 = 0;

  if (str0 == 0) { str0=0; len0=~0; }
  if (str1 == 0) { str1=0; len1=~0; }
  
  unsigned long start = millis();
  
  for (;;) {
    if (at0 == len0) return true;
    if (at1 == len1) return false;
    if (!available()) {
      if (millis()-start < timeout) {
        delayMicroseconds(1);
        continue;
      }
      return false;
    }
    char c = peek();
    if (str0 != 0) {
      if (c == str0[at0]) {
        ++at0;
      } else {
        int at;
        for (at=at0; at > 0; --at) {
          if (str0[at-1] == c && strncmp(str0,str0+(at0-at),at0-at) == 0) { break; }
        }
        at0=at;
      }
    }
    if (str1 != 0) {
      if (c == str1[at1]) {
        ++at1;
      } else {
        int at;
        for (at=at1; at > 0; --at) {
          if (str1[at-1] == c && strncmp(str1,str1+(at1-at),at1-at) == 0) { break; }
        }
        at1=at;
      }
    }
    read();
  }
}

long Stream::parseInt(LookaheadMode lookahead, char ignore)
{
  long value = 0;
  bool neg = false;
  int c;
  int state = 0;
  unsigned long start = millis();

  for (;;) {
    if (!available()) {
      if (millis()-start < timeout) {
        delayMicroseconds(1);
        continue;
      }
      return neg ? -value : value;
    }
    c = peek();
    if (c == ignore) { read(); continue; }

    switch(state) {
    case 0:
      if (lookahead == SKIP_WHITESPACE && isblank(c)) { read(); continue; }
      else if (lookahead == SKIP_ALL && ! (((c >= '0' && c <= '9') || (c=='-')))) { read(); continue; }
      else if (! (((c >= '0' && c <= '9') || (c=='-')))) { return neg ? -value : value; }
      if (c == '-') { neg=true; read(); state = 1; continue; } else { state = 1; continue; }
      break;
    case 1:
      if (c >= '0' && c <= '9') {
        value = value*10 + (c-'0');
        read();
        continue;
      } else {
        return neg ? -value : value;
      }
    }
  }
}

float Stream::parseFloat(LookaheadMode lookahead, char ignore)
{
  double value = 0, den = 1;
  bool neg = false;
  int c;
  int state = 0;
  unsigned long start = millis();

  for (;;) {
    if (!available()) {
      if (millis()-start < timeout) {
        delayMicroseconds(1);
        continue;
      }
      return neg ? -value : value;
    }

    c = peek();
    if (c == ignore) { read(); continue; }

    switch(state) {
    case 0:
      if (lookahead == SKIP_WHITESPACE && isblank(c)) { read(); continue; }
      else if (lookahead == SKIP_ALL && ! (((c >= '0' && c <= '9') || (c=='-'||c=='.')))) { read(); continue; }
      else if (! (((c >= '0' && c <= '9') || (c=='-'||c=='.')))) { return value; }
      if (c == '-') { neg=true; read(); state = 1; continue; } else { state = 1; continue; }
      break;
    case 1:
      if (c >= '0' && c <= '9') {
        value = value*10 + (c-'0');
        read();
        continue;
      } else if (c == '.') {
        read();
        state = 2;
        continue;
      } else {
        return neg ? -value : value;
      }
    case 2:
      if (c >= '0' && c <= '9') {
        den=10*den;
        value = value + (c-'0')/den;
        read();
        continue;
      } else {
        return neg ? -value : value;
      }
    }
  }
}

size_t Stream::readBytes(char *buffer, size_t length)
{
  unsigned long start = millis();
  size_t size = 0;

  for (;;) {
    if (size >= length) return size;
    if (!available()) {
      if (millis()-start < timeout) {
        delayMicroseconds(1);
        continue;
      }
      return size;
    }
    buffer[size++]=read();
  }
}


size_t Stream::readBytesUntil(char terminator, char *buffer, size_t length)
{
  unsigned long start = millis();
  size_t size = 0;

  for (;;) {
    if (size >= length) return size;
    if (!available()) {
      if (millis()-start < timeout) {
        delayMicroseconds(1);
        continue;
      }
      return size;
    }
    if (peek() == terminator) return size;
    buffer[size++]=read();
  }
}


String Stream::readString()
{
  unsigned long start = millis();  
  String ans;
  for (;;) {
    if (!available()) {
      if (millis()-start < timeout) {
        delayMicroseconds(1);
        continue;
      }
      return ans;
    }
    ans.concat((char) read());
  }
}

String Stream::readStringUntil(char terminator)
{
  unsigned long start = millis();  
  String ans;
  for (;;) {
    if (!available()) {
      if (millis()-start < timeout) {
        delayMicroseconds(1);
        continue;
      }
      return ans;
    }
    if (peek() == terminator) return ans;
    ans.concat((char) read());
  }
}

CppIOStream::CppIOStream(std::istream &_in, std::ostream &_out) : in(_in), out(_out) {}

int CppIOStream::available() {
  if (buffered.size() < 32) {
    char tmp[32];
    size_t status = in.readsome(tmp,sizeof(tmp));
    if (status > 0) {
      buffered.append(tmp,status);
    }
  }
  return buffered.size();
}

int CppIOStream::read() {
  if (buffered.size() > 0) {
    int ans = buffered[0];
    buffered.erase(0,1);
    return ans;
  }
  return in.get();
}

int CppIOStream::peek() {
  return in.peek();
}

size_t CppIOStream::write(uint8_t c) {
  char tmp[1];
  tmp[0]=c;
  out.write(tmp,1);
  return 1;
}

size_t CppIOStream::write(const uint8_t *buffer, size_t size) {
  out.write((const char *)buffer,size);
  return size;
}

void CppIOStream::begin(long baud) { (void) baud; }
bool CppIOStream::operator!() const { return false; }

#endif


MockStream::MockStream() {}
MockStream::MockStream(const char *_input) : input(_input) {}
MockStream::MockStream(const __FlashStringHelper *_input) : input(_input) {}
MockStream::MockStream(const String &_input) : input(_input) {}

size_t MockStream::write(uint8_t x) { return output.write(x); }
size_t MockStream::write(const uint8_t *buffer, size_t size) { return output.write(buffer,size); }
int MockStream::availableForWrite() { return output.availableForWrite(); }
int MockStream::available() { return input.length(); }
int MockStream::read() {
  int ans = peek();
  if (input.length() > 0) { input.remove(0,1); }
  return ans;
}

int MockStream::peek() {
  return input.length() > 0 ? input[0] : -1;
}

void MockStream::begin(long baud) { (void) baud; }
bool MockStream::operator!() const { return false; }
void MockStream::flush() { }

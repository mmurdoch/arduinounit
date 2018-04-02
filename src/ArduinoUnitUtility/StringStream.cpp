#include "StringStream.h"

size_t StringStream::write(uint8_t c) {
  concat((char) c);
  return 1;
}

size_t StringStream::write(const uint8_t *buffer, size_t size) {
  concat((const char*)buffer,size);
  return size;
}

StringStream::StringStream() { }
StringStream::StringStream(const String &init) : String(init) { }
StringStream::StringStream(const char *init) : String(init) { }
StringStream::StringStream(const __FlashStringHelper *init) : String(init) { }
int StringStream::available() { return length(); }
int StringStream::read() {
  if (length() > 0) {
    int ans = (*this)[0];
    remove(0);
    return ans;
  } else {
    return -1;
  }
}
  
int StringStream::peek() {
  if (length() > 0) {
    return (*this)[0];
  } else {
    return -1;
  }
}

void StringStream::baud(long rate) { (void) rate; }
bool StringStream::operator!() const { return false; }



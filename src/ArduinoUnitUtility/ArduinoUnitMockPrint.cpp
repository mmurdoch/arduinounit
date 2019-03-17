#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include "ArduinoUnitMockPrint.h"
#if defined(ARDUINO)
#include <Print.h>
#else
#include <iostream>
#include <math.h>

#ifdef DEC
#undef DEC
#endif
#define DEC 10

#ifdef HEX
#undef HEX
#endif
#define HEX 16

#ifdef OCT
#undef OCT
#endif
#define OCT 8

#ifdef BIN
#undef BIN
#endif
#define BIN 2

size_t Print::printNumber(unsigned long value, unsigned char base, bool isSigned) {
  int sz = 0;
  bool neg = isSigned && (((long) value) < 0);
  if (neg) { ++sz; value = -value; }
  unsigned long x = value;
  do { ++sz; x=x/base; } while (x != 0);
  char buffer[sz+1];
  buffer[sz]=0;
  int at = sz;
  if (neg) { buffer[0]='-'; }

  x=value;
  do {
    int d = x % base;
    char c = (d < 10) ? d+'0' : d+'a'-10;
    buffer[--at]=c;
    x = x/base;
  } while (x != 0);
  return print(buffer);
}

size_t Print::printFloat(double value, uint8_t decimalPoints) {
  int sz = log10(fabs(value)+1)+1+decimalPoints+4;
  char tmp[sz];
  snprintf(tmp,sz,"%1.*lf",decimalPoints,value);
  return write((const uint8_t*)tmp,strlen(tmp));
}

void Print::setWriteError(int err) { write_error = err; }

Print::Print() : write_error(0) {}
  
int Print::getWriteError() { return write_error; }
void Print::clearWriteError() { setWriteError(0); }
  
size_t Print::write(const char *x) {
  if (x == NULL) return 0;
  return write((const uint8_t *)x, strlen(x));
}

size_t Print::write(const uint8_t *buffer, size_t size) {
  for (int i=0; i<size; ++i) {
    if (write(buffer[i]) != 1) return i;
  }
  return size;
}

size_t Print::write(const char *buffer, size_t size) {
  return write((const uint8_t *)buffer, size);
}

int Print::availableForWrite() { return 0; }

size_t Print::print(const __FlashStringHelper *x) { return write((const uint8_t*)x,strlen((const char*)x)); }
size_t Print::print(const String &x) { return write(x.c_str(),x.length()); }
size_t Print::print(const char x[]) { return write((const uint8_t *)x,strlen((const char*)x)); }
size_t Print::print(char x) { return write(x); }
size_t Print::print(unsigned char x, int base) { return printNumber(x,base); }
size_t Print::print(int x, int base) {  return printNumber(x,base,true); }
size_t Print::print(unsigned int x, int base) { return printNumber(x,base); }
size_t Print::print(long x, int base) { return printNumber(x,base,true); }
size_t Print::print(unsigned long x, int base) { return printNumber(x,base); }
size_t Print::print(double x, int decimalPoints) { return printFloat(x,decimalPoints); }
size_t Print::print(const Printable& x) { return x.printTo(*this); }
size_t Print::println(const __FlashStringHelper *x) { return print(x) + println(); }
size_t Print::println(const String &x) { return print(x) + println(); }
size_t Print::println(const char x[]) { return print(x) + println(); }
size_t Print::println(char x) { return print(x) + println(); }
size_t Print::println(unsigned char x, int base) { return print(x,base) + println(); }
size_t Print::println(int x, int base) { return print(x,base) + println(); }
size_t Print::println(unsigned int x, int base) { return print(x,base) + println(); }
size_t Print::println(long x, int base) { return print(x,base) + println(); }
size_t Print::println(unsigned long x, int base) { return print(x,base) + println(); }
size_t Print::println(double x, int decimalPoints) { return print(x,decimalPoints) + println(); }
size_t Print::println(const Printable &x) { return print(x) + println(); }
size_t Print::println() { return write((const uint8_t *)"\r\n",2); }
void Print::flush() {}


CppStreamPrint::CppStreamPrint(std::ostream &_out) : out(_out) {}

size_t CppStreamPrint::write(uint8_t c) {
  out.put((char) c);
  return 1;
}

size_t CppStreamPrint::write(const uint8_t *buffer, size_t size) {
  out.write((const char *)buffer,size);
  return size;
}

int CppStreamPrint::availableForWrite() {
  return INT_MAX;
}

#endif

MockPrint::MockPrint() {}
MockPrint::MockPrint(const char *_initial) : String(_initial) {}
MockPrint::MockPrint(const __FlashStringHelper *_initial) : String(_initial) {}
MockPrint::MockPrint(const String &_initial) : String(_initial) {}
MockPrint::~MockPrint() {}

size_t MockPrint::write(uint8_t x) { concat((char) x); return 1; }
size_t MockPrint::write(const uint8_t *buffer, size_t size) {
#ifdef TEENSYDUINO
  append((const char *)buffer, size);
#else
  concat((const char *)buffer, size);
#endif  // TEENSYDUINO
  return size;
}
int MockPrint::availableForWrite() { return INT_MAX; }

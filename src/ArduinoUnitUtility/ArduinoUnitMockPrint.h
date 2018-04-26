#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#if !defined(ARDUINO)
#include <iostream>
#include <sstream>
#endif
#include "ArduinoUnitMockPrintable.h"
#include "ArduinoUnitMockWString.h"

#if defined(ARDUINO)
#include <Print.h>
#include <WString.h>
#else
#include <iostream>

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

class Print
{
  
 private: int write_error;
 private: size_t printNumber(unsigned long x, uint8_t base, bool isSigned = false);
 private: size_t printFloat(double x, uint8_t decimalPoints = 2);
  
 protected: void setWriteError(int err = 1);
  
 public: Print();
  
 public: int getWriteError();
 public: void clearWriteError();
  
 public: virtual size_t write(uint8_t) = 0;
 public: size_t write(const char *str);
 public: virtual size_t write(const uint8_t *buffer, size_t size);
 public: size_t write(const char *buffer, size_t size);
 public: virtual int availableForWrite();
 public: size_t print(const __FlashStringHelper *x);
 public: size_t print(const String &x);
 public: size_t print(const char x []);
 public: size_t print(char x);
 public: size_t print(unsigned char x, int base = DEC);
 public: size_t print(int x, int base = DEC);
 public: size_t print(unsigned int x, int base = DEC);
 public: size_t print(long x, int base = DEC);
 public: size_t print(unsigned long x, int base = DEC);
 public: size_t print(double x, int decimalPoints = 2);
 public: size_t print(const Printable& x);

 public: size_t println(const __FlashStringHelper *x);
 public: size_t println(const String &x);
 public: size_t println(const char x[]);
 public: size_t println(char x);
 public: size_t println(unsigned char x, int base = DEC);
 public: size_t println(int x, int base = DEC);
 public: size_t println(unsigned int x, int base = DEC);
 public: size_t println(long x, int base = DEC);
 public: size_t println(unsigned long x, int base = DEC);
 public: size_t println(double x, int decimalPoints = 2);
 public: size_t println(const Printable& x);
 public: size_t println();
 public: template <typename T> size_t print(const T &x) {
    std::ostringstream oss;
    oss << x;
    const std::string &s=oss.str();
    print(s.c_str());
    return s.length();
  }

 public: template <typename T> size_t println(const T &x) {
    return print(x)+println();
  }

 public: virtual void flush();
};

struct CppStreamPrint : Print {
  std::ostream &out;
  CppStreamPrint(std::ostream &_out = std::cout);
  size_t write(uint8_t c);
  size_t write(const uint8_t *buffer, size_t size);
  int availableForWrite();
};


#endif

struct MockPrint : virtual Print, virtual String {
  MockPrint();
  MockPrint(const char *_initial);
  MockPrint(const __FlashStringHelper *_initial);
  MockPrint(const String &_initial);
  virtual size_t write(uint8_t x);
  virtual size_t write(const uint8_t *buffer, size_t size);
  virtual int availableForWrite();
  virtual ~MockPrint();
};

#include "ArduinoUnitWiden.h"
template< > struct ArduinoUnitWiden < MockPrint > { typedef ArduinoUnitString type; };

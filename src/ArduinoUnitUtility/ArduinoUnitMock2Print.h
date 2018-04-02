#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#if defined(ARDUINO)
#include <Print.h>
#else
#include <iostream>
#include "ArduinoUnitMock2Printable.h"
#include "ArduinoUnitMock2WString.h"

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
  private:
  int write_error;
  size_t printNumber(unsigned long x, uint8_t base, bool isSigned = false);
  size_t printFloat(double x, uint8_t decimalPoints = 2);
  protected:
  void setWriteError(int err = 1);
  public:
  Print();
  
  int getWriteError();
  void clearWriteError();
  
  virtual size_t write(uint8_t) = 0;
  size_t write(const char *str);
  virtual size_t write(const uint8_t *buffer, size_t size);
  size_t write(const char *buffer, size_t size);
  virtual int availableForWrite();
  size_t print(const __FlashStringHelper *x);
  size_t print(const String &x);
  size_t print(const char x []);
  size_t print(char x);
  size_t print(unsigned char x, int base = DEC);
  size_t print(int x, int base = DEC);
  size_t print(unsigned int x, int base = DEC);
  size_t print(long x, int base = DEC);
  size_t print(unsigned long x, int base = DEC);
  size_t print(double x, int decimalPoints = 2);
  size_t print(const Printable& x);

  size_t println(const __FlashStringHelper *x);
  size_t println(const String &x);
  size_t println(const char x[]);
  size_t println(char x);
  size_t println(unsigned char x, int base = DEC);
  size_t println(int x, int base = DEC);
  size_t println(unsigned int x, int base = DEC);
  size_t println(long x, int base = DEC);
  size_t println(unsigned long x, int base = DEC);
  size_t println(double x, int decimalPoints = 2);
  size_t println(const Printable& x);
  size_t println();

  virtual void flush();
};

struct CppStreamPrint : Print {
  std::ostream &out;
  CppStreamPrint(std::ostream &_out = std::cout);
  size_t write(uint8_t c);
  size_t write(const uint8_t *buffer, size_t size);
};


#endif

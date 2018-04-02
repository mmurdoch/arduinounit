#pragma once

#include <stddef.h>
#include <stdint.h>

#if defined(ARDUINO)
#include "Print.h"
#include "WString.h"
#else
#include <iostream>
#include <sstream>
#include <string>
#endif

#if defined(DEC)
#undef DEC
#endif
#define DEC 10

#if defined(HEX)
#undef HEX
#endif
#define HEX 16

#if defined(BIN)
#undef BIN
#endif
#define BIN 2

class StringPrint
#if defined(ARDUINO)
  : public String, public Print, 
#else
  : public std::ostringstream
#endif
{
 public: virtual size_t write(const uint8_t *buffer, size_t size) {
#if defined(ARDUIINO)
    concat(buffer,size);
    return size;
#else
    std::ostringstream::write(buffer,size);
    return size;
#endif
  }

#

#if !defined(ARDUIINO)
  template <typename T>
    public: size_t print(const T &x) {
    size_t begin = length();
    (*this)  << x;
    size_t end = length();    
    return end-begin;
  }
#endif

template <typename T>
    public: size_t println(const T &x) {
#if defined(ARDUIINO)
    return Print::println(x);
#else
    size_t begin = length();
    (*this)  << x << std::endl;
    size_t end = length();    
    return end-begin;
#endif
  }
  
  template <typename T>
    public: size_t print(const T &x, uint8_t digits) {
#if defined(ARDUIINO)
    return Print::print(x);
#else
    std::ostringstream oss;
    oss << x;
    std::string s = oss.str();
    append(x);
    return s.length();
#endif
  }

#if !defined(ARDUINO)
  template <typename T>
    public: void concat(const T &x) { append(x); }
#endif
  uint8_t compareTo(const ArduinoUnitString &s) const {
    return ArduinoUnitString(*this).compareTo(s);
  }
};


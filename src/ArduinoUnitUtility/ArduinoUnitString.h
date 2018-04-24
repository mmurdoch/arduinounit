#pragma once

#include <stdint.h>
#if defined(ARDUINO)
#include <WString.h>
#include <Print.h>
#else
#include <string.h> // memcpy, memset, strlen, strcmp
#include <iostream>
#include <string>
#endif

#include "Flash.h"
#include "Compare.h"

#include "ArduinoUnitMockWString.h"
#include "ArduinoUnitMockPrint.h"

class ArduinoUnitString : public Printable
{
 public:
  const char * const data;
#if defined(PROGMEM)
  const bool inFlash;
#endif
 public:

  ArduinoUnitString(const char *_data);
  ArduinoUnitString(const __FlashStringHelper *_data);
  ArduinoUnitString(const String &_data);
#if !defined(ARDUINO)
  ArduinoUnitString(const std::string &_data);
#endif
  void readTo(void *destination, uint16_t offset, uint8_t length) const;
  uint16_t length() const;
  int8_t compareTo(const ArduinoUnitString &to) const;
  size_t printTo(Print &p) const;
  bool matches(const char *pattern) const;

  inline bool operator<(const ArduinoUnitString &to) const {
    return compareTo(to) < 0;
  }
  inline bool operator<=(const ArduinoUnitString &to) const {
    return compareTo(to) <= 0;
  }
  inline bool operator==(const ArduinoUnitString &to) const {
    return compareTo(to) == 0;
  }
  inline bool operator!=(const ArduinoUnitString &to) const {
    return compareTo(to) != 0;
  }
  inline bool operator>=(const ArduinoUnitString &to) const {
    return compareTo(to) >= 0;
  }
  inline bool operator>(const ArduinoUnitString &to) const {
    return compareTo(to) > 0;
  }
};

template  <  > struct Compare<ArduinoUnitString,ArduinoUnitString>
{
  inline static int8_t between(const ArduinoUnitString &a,const ArduinoUnitString &b)
  {
    return a.compareTo(b);
  } // between
  inline static bool equal(const ArduinoUnitString &a,const ArduinoUnitString &b)
  {
    return a.compareTo(b) == 0;
  } // equal
  inline static bool notEqual(const ArduinoUnitString &a,const ArduinoUnitString &b)
  {
    return a.compareTo(b) != 0;
  } // notEqual
  inline static bool less(const ArduinoUnitString &a,const ArduinoUnitString &b)
  {
    return a.compareTo(b) < 0;
  } // less
  inline static bool more(const ArduinoUnitString &a,const ArduinoUnitString &b)
  {
    return a.compareTo(b) > 0;
  } // more
  inline static bool lessOrEqual(const ArduinoUnitString &a,const ArduinoUnitString &b)
  {
    return a.compareTo(b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const ArduinoUnitString &a,const ArduinoUnitString &b)
  {
    return a.compareTo(b) >= 0;
  } // moreOrEqual
};

#if !defined(ARDUINO)
std::ostream& operator<<(std::ostream & out, const ArduinoUnitString &s);
#endif


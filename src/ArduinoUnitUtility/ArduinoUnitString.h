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

class ArduinoUnitString
#if defined(ARDUINO)
: public Printable
#endif
{
 public:
  const char * const data;
#if ARDUINO_UNIT_USE_FLASH  > 0
  const bool inFlash;
#endif
 public:

  ArduinoUnitString(const char *_data);

#if ARDUINO_UNIT_USE_FLASH  > 0
  ArduinoUnitString(const __FlashStringHelper *_data);
#endif

#if defined(ARDUINO)
  ArduinoUnitString(const String &_data);
#else
  ArduinoUnitString(const std::string &_data);  
#endif

#if ARDUINO_UNIT_USE_FLASH  > 0  
  bool inline flash() const { return inFlash; }
#else
  bool inline flash() const { return false; }
#endif
  void readTo(void *destination, uint16_t offset, uint8_t length) const;
  uint16_t length() const;
  int8_t compareTo(const ArduinoUnitString &to) const;
#if defined(ARDUINO)
  size_t printTo(Print &p) const;
#endif
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


#if ! defined(ARDUINO)
std::ostream & operator<<(std::ostream &out, const ArduinoUnitString &value);
#endif

#pragma once

#include <stdint.h>
#include "Flash.h"

#if defined(ARDUINO)
#include <WString.h>
#include <Print.h>
#else
#include <iostream>
#endif

class ArduinoUnitString
#if defined(ARDUINO)
: public Printable
#endif
{
 public:
#if ARDUINO_UNIT_USE_FLASH  > 0
  const uint32_t data;
  ArduinoUnitString(const __FlashStringHelper *_data);
  ArduinoUnitString(const char *_data);
#if defined(ARDUINO)
  ArduinoUnitString(const String &_data);
#endif
#else
  const char *data;
  ArduinoUnitString(const char *_data);
#if defined(ARDUINO)
  ArduinoUnitString(const String &_data);
#endif
#endif
#if ARDUINO_UNIT_USE_FLASH  > 0  
  bool inline flash() const { return ((data & 0x80000000) != 0); };
#else
  bool inline flash() const { return false; }
#endif
  void read(void *destination, uint16_t offset, uint8_t length) const;
  uint16_t length() const;
  int8_t compare(const ArduinoUnitString &to) const;
#if defined(ARDUINO)
  size_t printTo(Print &p) const;
#endif
  bool matches(const char *pattern) const;
};

#if ! defined(ARDUINO)
std::ostream & operator<<(std::ostream &out, const ArduinoUnitString &value);
#endif



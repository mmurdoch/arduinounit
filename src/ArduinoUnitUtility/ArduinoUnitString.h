#pragma once

#include <stdint.h>
#include <WString.h>
#include <Print.h>
#include <ArduinoUnitUtility/Flash.h>

class ArduinoUnitString : public Printable {
 public:
#if ARDUINO_UNIT_USE_FLASH  > 0
  const uint32_t data;
  ArduinoUnitString(const __FlashStringHelper *_data);
  ArduinoUnitString(const char *_data);
  ArduinoUnitString(const String &_data);
#else
  const char *data;
  ArduinoUnitString(const char *_data);
  ArduinoUnitString(const String &_data);
#endif
#if ARDUINO_UNIT_USE_FLASH  > 0  
  boolean inline flash() const { return ((data & 0x80000000) != 0); };
#else
  boolean inline flash() const { return false; }
#endif
  void read(void *destination, uint16_t offset, uint8_t length) const;
  uint16_t length() const;
  int8_t compare(const ArduinoUnitString &to) const;
  size_t printTo(Print &p) const;
  bool matches(const char *pattern) const;
};



#pragma once

#include <stdint.h>
#include <WString.h>
#include <Print.h>
#include <ArduinoUnitUtility/Flash.h>

class ArduinoUnitString : public Printable {
 public:
  const uint32_t data;
  ArduinoUnitString(const char *_data);
  ArduinoUnitString(const __FlashStringHelper *_data);
  void read(void *destination, uint16_t offset, uint8_t length) const;
  uint16_t length() const;
  int8_t compare(const ArduinoUnitString &to) const;
  size_t printTo(Print &p) const;
  bool matches(const char *pattern) const;
};



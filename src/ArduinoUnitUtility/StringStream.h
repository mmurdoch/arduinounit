#pragma once

#include "ArduinoUnitMockWString.h"
#include "ArduinoUnitMockStream.h"

struct StringStream : Stream, String {
  size_t write(uint8_t c);
  size_t write(const uint8_t *buffer, size_t size);
  StringStream();
  StringStream(const String &init);
  StringStream(const char *init);
  StringStream(const __FlashStringHelper *init);
  int available();
  virtual int read();
  virtual int peek();
  virtual void baud(long rate);
  virtual bool operator!() const;
};

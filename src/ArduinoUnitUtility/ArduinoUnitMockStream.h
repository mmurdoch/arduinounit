#pragma once

#include <stddef.h>
#include <stdint.h>
#include "ArduinoUnitMockWString.h"
#include "ArduinoUnitMockPrint.h"

#if defined(ARDUINO)
#include <Stream.h>
#else
#include <string>
#include <iostream>

enum LookaheadMode { SKIP_ALL, SKIP_NONE, SKIP_WHITESPACE  };

// Just the public parts
class Stream : public Print
{
 private: unsigned long timeout;
 private: enum { NO_IGNORE_CHAR=0x01 };
 private: enum { DEFAULT_TIMEOUT=1000 };
  
 public: virtual int available() = 0;
 public: virtual int read() = 0;
 public: virtual int peek() = 0;

 public: Stream();

 public: void setTimeout(unsigned long timeout);
 public: unsigned long getTimeout() const;
  
 public: bool find(char *target);
 public: bool find(uint8_t *target);
 public: bool find(char *target, size_t length);
 public: bool find(uint8_t *target, size_t length);
 public: bool find(char target);
 public: bool findUntil(char *target, char *terminator);
 public: bool findUntil(uint8_t *target, char *terminator);

 public: bool findUntil(char *target, size_t targetLen, char *terminate, size_t termLen);
 public: bool findUntil(uint8_t *target, size_t targetLen, char *terminate, size_t termLen);

 public: long parseInt(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR);
 public: float parseFloat(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR);
 public: size_t readBytes(char *buffer, size_t length);
 public: size_t readBytes(uint8_t *buffer, size_t length);
 public: size_t readBytesUntil(char terminator, char *buffer, size_t length);
 public: size_t readBytesUntil(char terminator, uint8_t *buffer, size_t length);
 public: String readString();
 public: String readStringUntil(char terminator);
};

struct CppIOStream : Stream {
  std::istream &in;
  std::ostream &out;
  std::string buffered;
  
  CppIOStream(std::istream &_in = std::cin, std::ostream &_out = std::cout);

  int available();
  int read();
  int peek();
  size_t write(uint8_t c);
  size_t write(const uint8_t *buffer, size_t size);

  virtual void begin(long baud);
  virtual bool operator!() const;
};


#endif

// MockStream is like MockPrint, but also reads from input (which is a mockstream as well)

struct MockStream : Stream {
  MockPrint input;
  MockPrint output;

  MockStream();
  MockStream(const char *_input);
  MockStream(const __FlashStringHelper *_input);
  MockStream(const String &_input);
  
  virtual size_t write(uint8_t x);
  virtual size_t write(const uint8_t *buffer, size_t size);
  virtual int availableForWrite();
  virtual int available();
  virtual int read();
  virtual int peek();
  virtual void begin(long baud);
  virtual bool operator!() const;
  virtual void flush();
};

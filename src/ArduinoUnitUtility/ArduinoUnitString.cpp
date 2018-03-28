#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <iostream>
#endif

#include "ArduinoUnitString.h"

#if ARDUINO_UNIT_USE_FLASH > 0
#define FLASH_MASK(...) __VA_ARGS__
#define NO_FLASH_MASK(...)
#else
#define FLASH_MASK(...)
#define NO_FLASH_MASK(...) __VA_ARGS__
#endif

ArduinoUnitString::ArduinoUnitString(const char *_data) : data(_data) FLASH_MASK(,inFlash(false)) {}


#if ARDUINO_UNIT_USE_FLASH > 0
ArduinoUnitString::ArduinoUnitString(const __FlashStringHelper *_data) : data((const char *)_data), inFlash(true) {}
#endif

#if defined(ARDUINO)
ArduinoUnitString::ArduinoUnitString(const String &_data) : data(_data.c_str()) FLASH_MASK(,inFlash(false)) {}
#else
ArduinoUnitString::ArduinoUnitString(const std::string &_data) : data(_data.c_str()) FLASH_MASK(,inFlash(false)) {}
#endif

void ArduinoUnitString::readTo(void *destination, uint16_t offset, uint8_t length) const
{
#if ARDUINO_UNIT_USE_FLASH  > 0
  if (inFlash) {
    memcpy_P(destination,data+offset,length);
  } else {
    memcpy(destination,data+offset,length);
  }
#else
  memcpy(destination,data+offset,length);
#endif
}

uint16_t ArduinoUnitString::length() const {
#if ARDUINO_UNIT_USE_FLASH  > 0
  if (inFlash) {
    return strlen_P(data);
  } else {
    return strlen(data);
  }
#else
  return strlen(data);  
#endif  
}

int8_t ArduinoUnitString::compareTo(const ArduinoUnitString &to) const
{
#if ARDUINO_UNIT_USE_FLASH  > 0
  switch ((inFlash?2:0)|(to.inFlash?1:0)) {
  case 0:
    {
      int ans = strcmp(data,to.data);
      return (ans == 0) ? 0 : (ans > 0) ? 1 : -1;
    }
  case 1:
    {
      int ans = strcmp_P(data,to.data);
      return (ans == 0) ? 0 : (ans > 0) ? 1 : -1;
    }
  case 2:
    {
      int ans = -strcmp_P(to.data,data);
      return (ans == 0) ? 0 : (ans > 0) ? 1 : -1;
    }
  default:
    uint8_t a_buf[4],b_buf[4];
    const char *a_ptr = data;
    const char *b_ptr = to.data;
    uint16_t i=0;
  
    for (;;) {
      uint8_t j=(i%4);
      if (j == 0) {
        memcpy_P(a_buf,a_ptr,sizeof(a_buf));
        a_ptr += sizeof(a_buf);
        memcpy_P(b_buf,b_ptr,sizeof(b_buf));
        b_ptr += sizeof(b_buf);
      }
      uint8_t aj=a_buf[j];
      uint8_t bj=b_buf[j];
      if (aj < bj) return -1;
      if (bj < aj) return  1;
      if (aj == 0) return  0;
      ++i;
    }
  }
#else
  int ans = strcmp(data,to.data); 
  return (ans == 0) ? 0 : (ans > 0) ? 1 : -1;
#endif
}

#if defined(ARDUINO)
size_t ArduinoUnitString::printTo(Print &p) const {
#if ARDUINO_UNIT_USE_FLASH  > 0
  if (inFlash) {
    return p.print((const __FlashStringHelper *)data);
  } else {
    return p.print(data);
  }
#else
  return p.print(data);  
#endif
}
#else
std::ostream & operator<<(std::ostream &out, const ArduinoUnitString &value) {
#if ARDUINO_UNIT_USE_FLASH  > 0
  if (inFlash) {
    char tmp[128];
    int n=value.length();
    int m;
    for (int i=0; i<n; i += m) {
      int m=sizeof(tmp);
      if (i+m > n) m=n-i;
      value.readTo(tmp,i,m);
      out.write(tmp,m);
    }
  } else {
    out << value.data;
  }
#else
  out << value.data;
#endif
  return out;
}
#endif


bool ArduinoUnitString::matches(const char *pattern) const {
  uint8_t np = strlen(pattern);
  uint8_t ns = length();
  uint8_t nb = (np+2)/8+((np+2)%8 != 0);
  uint8_t k;
  uint8_t buffer[8];

  uint8_t buffer0[nb];
  uint8_t buffer1[nb];

  uint8_t *state0=buffer0;
  uint8_t *state1=buffer1;

  memset(state0,0,nb);
  state0[0]=1;
  for (k=1; pattern[k-1] == '*'; ++k) state0[k/8] |= (1 << (k%8));

  for (int i=0; i<=ns; ++i) {
    if ((i%sizeof(buffer)) == 0) {
      uint8_t n=sizeof(buffer);
      if (ns+1-i < n) n=ns+1-i;
      readTo(buffer,i,n);
    }
    uint8_t c=buffer[i%sizeof(buffer)];

    memset(state1,0,nb);
    for (int j=0; j<=np; ++j) {
      if (state0[j/8] & (1 << (j%8))) {
        if (pattern[j] == '*') {
          k=j;
          state1[k/8] |= (1 << (k%8));
          ++k;
          state1[k/8] |= (1 << (k%8));
        } else if (pattern[j] == c) {
          k=j+1;
          state1[k/8] |= (1 << (k%8));
          while (pattern[k] == '*') {
            ++k; 
            state1[k/8] |= (1 << (k%8));
          }
        }
      }
    }

    uint8_t *tmp = state0;
    state0 = state1;
    state1 = tmp;
  }

  k=np+1;
  return (state0[k/8]&(1<<(k%8))) != 0;
}

#include <Arduino.h>
#include "ArduinoUnitUtility/ArduinoUnitString.h"

#if ARDUINO_UNIT_USE_FLASH  > 0
ArduinoUnitString::ArduinoUnitString(const __FlashStringHelper *_data) : data(0x80000000|(uint32_t)_data), debug(false) {}
ArduinoUnitString::ArduinoUnitString(const char *_data) : data((uint32_t)_data) , debug(false) {}
ArduinoUnitString::ArduinoUnitString(const String &_data) : data((uint32_t)_data.c_str()) {
  Serial.print("represented '");
  Serial.print(_data);
  Serial.print("' as '");
  printTo(Serial);
  Serial.println("'");
  debug = false;
}
#else
ArduinoUnitString::ArduinoUnitString(const char *_data) : data(_data) {}
ArduinoUnitString::ArduinoUnitString(const String &_data) : data(_data.c_str()) {}
#endif

void ArduinoUnitString::read(void *destination, uint16_t offset, uint8_t length) const
{
#if ARDUINO_UNIT_USE_FLASH  > 0
  if ((data & 0x80000000) != 0) {
    memcpy_P(destination,(const /* PROGMEM */ char *)((data+offset)&0x7FFFFFFF),length);
  } else {
    memcpy(destination,(char*)(data+offset),length);
  }
#else
  memcpy(destination,(char*)(data+offset),length);
#endif
}

uint16_t ArduinoUnitString::length() const {
#if ARDUINO_UNIT_USE_FLASH  > 0
  if ((data & 0x80000000) != 0) {
    return strlen_P((const /* PROGMEM */ char *)(data&0x7FFFFFFF));
  } else {
    return strlen((char*)(data));
  }
#else
  return strlen((char*)(data));  
#endif  
}

int8_t ArduinoUnitString::compare(const ArduinoUnitString &to) const
{
#if ARDUINO_UNIT_USE_FLASH  > 0
  switch ((flash()?2:0)|(to.flash()?1:0)) {
  case 0:
    {
      int ans = strcmp((const char *) data,(const char *) to.data);
      return (ans == 0) ? 0 : (ans > 0) ? 1 : -1;
    }
  case 1:
    {
      int ans = strcmp_P((const char *) data, (const /* PROGMEM */ char *)(to.data&0x7FFFFFFF));
      return (ans == 0) ? 0 : (ans > 0) ? 1 : -1;
    }
  case 2:
    {
      int ans = -strcmp_P((const char *) to.data, (const /* PROGMEM */ char *)(data&0x7FFFFFFF));
      return (ans == 0) ? 0 : (ans > 0) ? 1 : -1;
    }
  default:
    uint8_t a_buf[4],b_buf[4];
    const char *a_ptr = (const char *)(data&0x7FFFFFFF);
    const char *b_ptr = (const char *)(to.data&0x7FFFFFFF);
    uint16_t i=0;
  
    for (;;) {
      uint8_t j=(i%4);
      if (j == 0) {
        memcpy_P(a_buf,a_ptr,sizeof(a_buf));
        a_ptr += sizeof(a_buf);
        memcpy_P(b_buf,b_ptr,sizeof(b_buf));
        b_ptr += sizeof(b_buf);
      }
      if (a_buf[j] < b_buf[j]) return -1;
      if (a_buf[j] > b_buf[j]) return  1;
      if (a_buf[j] == 0) return 0;
      ++i;
    }
  }
#else
  int ans = strcmp(data,to.data); 
  return (ans == 0) ? 0 : (ans > 0) ? 1 : -1;
#endif
}

size_t ArduinoUnitString::printTo(Print &p) const {
#if ARDUINO_UNIT_USE_FLASH  > 0
  if ((data & 0x80000000) != 0) {
    return p.print((const __FlashStringHelper *)(data & 0x7FFFFFFF));
  } else {
    return p.print((char*)data);
  }
#else
  return p.print(data);  
#endif
}

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
      read(buffer,i,n);
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

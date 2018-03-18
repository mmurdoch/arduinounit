#include <Arduino.h>
#include <ArduinoUnitUtility/ArduinoUnitString.h>

ArduinoUnitString::ArduinoUnitString(const char *_data) : data((uint32_t)_data) {}
ArduinoUnitString::ArduinoUnitString(const __FlashStringHelper *_data) : data(0x80000000|(uint32_t)_data) {}
void ArduinoUnitString::read(void *destination, uint16_t offset, uint8_t length) const
{
  if ((data & 0x80000000) != 0) {
    memcpy_P(destination,(const /* PROGMEM */ char *)((data+offset)&0x7FFFFFFF),length);
  } else {
    memcpy(destination,(char*)(data+offset),length);
  }
}

uint16_t ArduinoUnitString::length() const {
  if ((data & 0x80000000) != 0) {
    return strlen_P((const /* PROGMEM */ char *)(data&0x7FFFFFFF));
  } else {
    return strlen((char*)(data));
  }
}

int8_t ArduinoUnitString::compare(const ArduinoUnitString &to) const
{
  uint8_t a_buf[4],b_buf[4];
  uint16_t i=0;
  
  for (;;) {
    uint8_t j=(i%4);
    if (j == 0) {
      this->read(a_buf,i,4);
      to.read(b_buf,i,4);
    }
    if (a_buf[j] < b_buf[j]) return -1;
    if (a_buf[j] > b_buf[j]) return  1;
    if (a_buf[j] == 0) return 0;
    ++i;
  }
}

size_t ArduinoUnitString::printTo(Print &p) const {
  if ((data & 0x80000000) != 0) {
    return p.print((const __FlashStringHelper *)(data & 0x7FFFFFFF));
  } else {
    return p.print((char*)data);
  }
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

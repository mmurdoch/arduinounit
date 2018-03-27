#pragma once
#include <stdint.h>
#include "ArduinoUnitUtility/Flash.h"
#include "ArduinoUnitUtility/ArduinoUnitWiden.h"
#include <WString.h>

template  < typename A, typename B > struct Compare
{
  inline static int8_t between(const A &a,const B &b)
  {
    if (a<b) return -1;
    if (b<a) return  1;
    return 0;
  } // between
  inline static bool equal(const A &a,const B &b)
  {
    return (!(a<b)) && (!(b<a));
  } // equal
  inline static bool notEqual(const A &a,const B &b)
  {
    return (a<b) || (b<a);
  } // notEqual
  inline static bool less(const A &a,const B &b)
  {
    return a<b;
  } // less
  inline static bool more(const A &a,const B &b)
  {
    return b<a;
  } // more
  inline static bool lessOrEqual(const A &a,const B &b)
  {
    return !(b<a);
  } // lessOrEqual
  inline static bool moreOrEqual(const A &a,const B &b)
  {
    return !(a<b);
  } // moreOrEqual
};
template  <  > struct Compare<String,String>;
template  <  > struct Compare<String,const char *>;
#if ARDUINO_UNIT_USE_FLASH  > 0
template  <  > struct Compare<String,const __FlashStringHelper *>;
#endif
template  <  > struct Compare<String,char *>;
template  < size_t M > struct Compare<String,char [M]>;
template  <  > struct Compare<const char *,String>;
template  <  > struct Compare<const char *,const char *>;
#if ARDUINO_UNIT_USE_FLASH  > 0
template  <  > struct Compare<const char *,const __FlashStringHelper *>;
#endif
template  <  > struct Compare<const char *,char *>;
template  < size_t M > struct Compare<const char *,char [M]>;
#if ARDUINO_UNIT_USE_FLASH  > 0
template  <  > struct Compare<const __FlashStringHelper *,String>;
#endif
#if ARDUINO_UNIT_USE_FLASH  > 0
template  <  > struct Compare<const __FlashStringHelper *,const char *>;
#endif
#if ARDUINO_UNIT_USE_FLASH  > 0
template  <  > struct Compare<const __FlashStringHelper *,const __FlashStringHelper *>;
#endif
#if ARDUINO_UNIT_USE_FLASH  > 0
template  <  > struct Compare<const __FlashStringHelper *,char *>;
#endif
#if ARDUINO_UNIT_USE_FLASH  > 0
template  < size_t M > struct Compare<const __FlashStringHelper *,char [M]>;
#endif
template  <  > struct Compare<char *,String>;
template  <  > struct Compare<char *,const char *>;
#if ARDUINO_UNIT_USE_FLASH  > 0
template  <  > struct Compare<char *,const __FlashStringHelper *>;
#endif
template  <  > struct Compare<char *,char *>;
template  < size_t M > struct Compare<char *,char [M]>;
template  < size_t N > struct Compare<char [N],String>;
template  < size_t N > struct Compare<char [N],const char *>;
#if ARDUINO_UNIT_USE_FLASH  > 0
template  < size_t N > struct Compare<char [N],const __FlashStringHelper *>;
#endif
template  < size_t N > struct Compare<char [N],char *>;
template  < size_t N, size_t M > struct Compare<char [N],char [M]>;
template  <  > struct Compare<String,String>
{
  inline static int8_t between(const String &a,const String &b)
  {
    int ans = a.compareTo(b);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(const String &a,const String &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const String &a,const String &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const String &a,const String &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const String &a,const String &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const String &a,const String &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const String &a,const String &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
template  <  > struct Compare<String,const char *>
{
  inline static int8_t between(const String &a,const char * const &b)
  {
    int ans = a.compareTo(b);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(const String &a,const char * const &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const String &a,const char * const &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const String &a,const char * const &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const String &a,const char * const &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const String &a,const char * const &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const String &a,const char * const &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
#if ARDUINO_UNIT_USE_FLASH  > 0
template  <  > struct Compare<String,const __FlashStringHelper *>
{
  inline static int8_t between(const String &a,const __FlashStringHelper * const &b)
  {
    return ArduinoUnitString(a).compare(ArduinoUnitString(b));
  } // between
  inline static bool equal(const String &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const String &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const String &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const String &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const String &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const String &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
#endif
template  <  > struct Compare<String,char *>
{
  inline static int8_t between(const String &a,char * const &b)
  {
    int ans = a.compareTo(b);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(const String &a,char * const &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const String &a,char * const &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const String &a,char * const &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const String &a,char * const &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const String &a,char * const &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const String &a,char * const &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
template  < size_t M > struct Compare<String,char [M]>
{
  inline static int8_t between(const String &a,const char (&b)[M])
  {
    int ans = a.compareTo(b);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(const String &a,const char (&b)[M])
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const String &a,const char (&b)[M])
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const String &a,const char (&b)[M])
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const String &a,const char (&b)[M])
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const String &a,const char (&b)[M])
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const String &a,const char (&b)[M])
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
template  <  > struct Compare<const char *,String>
{
  inline static int8_t between(const char * const &a,const String &b)
  {
    int ans = -b.compareTo(a);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(const char * const &a,const String &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const char * const &a,const String &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const char * const &a,const String &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const char * const &a,const String &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const char * const &a,const String &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const char * const &a,const String &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
template  <  > struct Compare<const char *,const char *>
{
  inline static int8_t between(const char * const &a,const char * const &b)
  {
    int ans = strcmp(a,b);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(const char * const &a,const char * const &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const char * const &a,const char * const &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const char * const &a,const char * const &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const char * const &a,const char * const &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const char * const &a,const char * const &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const char * const &a,const char * const &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
#if ARDUINO_UNIT_USE_FLASH  > 0
template  <  > struct Compare<const char *,const __FlashStringHelper *>
{
  inline static int8_t between(const char * const &a,const __FlashStringHelper * const &b)
  {
    int ans = strcmp_P(a,(const char *)b);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(const char * const &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const char * const &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const char * const &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const char * const &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const char * const &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const char * const &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
#endif
template  <  > struct Compare<const char *,char *>
{
  inline static int8_t between(const char * const &a,char * const &b)
  {
    int ans = strcmp(a,b);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(const char * const &a,char * const &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const char * const &a,char * const &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const char * const &a,char * const &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const char * const &a,char * const &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const char * const &a,char * const &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const char * const &a,char * const &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
template  < size_t M > struct Compare<const char *,char [M]>
{
  inline static int8_t between(const char * const &a,const char (&b)[M])
  {
    int ans = strcmp(a,b);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(const char * const &a,const char (&b)[M])
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const char * const &a,const char (&b)[M])
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const char * const &a,const char (&b)[M])
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const char * const &a,const char (&b)[M])
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const char * const &a,const char (&b)[M])
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const char * const &a,const char (&b)[M])
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
#if ARDUINO_UNIT_USE_FLASH  > 0
template  <  > struct Compare<const __FlashStringHelper *,String>
{
  inline static int8_t between(const __FlashStringHelper * const &a,const String &b)
  {
    return -Compare < String,const __FlashStringHelper * >::between(b,a);
  } // between
  inline static bool equal(const __FlashStringHelper * const &a,const String &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const __FlashStringHelper * const &a,const String &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const __FlashStringHelper * const &a,const String &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const __FlashStringHelper * const &a,const String &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const __FlashStringHelper * const &a,const String &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const __FlashStringHelper * const &a,const String &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
#endif
#if ARDUINO_UNIT_USE_FLASH  > 0
template  <  > struct Compare<const __FlashStringHelper *,const char *>
{
  inline static int8_t between(const __FlashStringHelper * const &a,const char * const &b)
  {
    int ans = -strcmp_P(b,(const char *)a);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(const __FlashStringHelper * const &a,const char * const &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const __FlashStringHelper * const &a,const char * const &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const __FlashStringHelper * const &a,const char * const &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const __FlashStringHelper * const &a,const char * const &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const __FlashStringHelper * const &a,const char * const &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const __FlashStringHelper * const &a,const char * const &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
#endif
#if ARDUINO_UNIT_USE_FLASH  > 0
template  <  > struct Compare<const __FlashStringHelper *,const __FlashStringHelper *>
{
  inline static int8_t between(const __FlashStringHelper * const &a,const __FlashStringHelper * const &b)
  {
    return ArduinoUnitString(a).compare(ArduinoUnitString(b));
  } // between
  inline static bool equal(const __FlashStringHelper * const &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const __FlashStringHelper * const &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const __FlashStringHelper * const &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const __FlashStringHelper * const &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const __FlashStringHelper * const &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const __FlashStringHelper * const &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
#endif
#if ARDUINO_UNIT_USE_FLASH  > 0
template  <  > struct Compare<const __FlashStringHelper *,char *>
{
  inline static int8_t between(const __FlashStringHelper * const &a,char * const &b)
  {
    int ans = -strcmp_P(b,(const char *)a);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(const __FlashStringHelper * const &a,char * const &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const __FlashStringHelper * const &a,char * const &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const __FlashStringHelper * const &a,char * const &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const __FlashStringHelper * const &a,char * const &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const __FlashStringHelper * const &a,char * const &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const __FlashStringHelper * const &a,char * const &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
#endif
#if ARDUINO_UNIT_USE_FLASH  > 0
template  < size_t M > struct Compare<const __FlashStringHelper *,char [M]>
{
  inline static int8_t between(const __FlashStringHelper * const &a,const char (&b)[M])
  {
    int ans = -strcmp_P(b,(const char *)a);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(const __FlashStringHelper * const &a,const char (&b)[M])
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const __FlashStringHelper * const &a,const char (&b)[M])
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const __FlashStringHelper * const &a,const char (&b)[M])
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const __FlashStringHelper * const &a,const char (&b)[M])
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const __FlashStringHelper * const &a,const char (&b)[M])
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const __FlashStringHelper * const &a,const char (&b)[M])
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
#endif
template  <  > struct Compare<char *,String>
{
  inline static int8_t between(char * const &a,const String &b)
  {
    int ans = -b.compareTo(a);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(char * const &a,const String &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(char * const &a,const String &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(char * const &a,const String &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(char * const &a,const String &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(char * const &a,const String &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(char * const &a,const String &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
template  <  > struct Compare<char *,const char *>
{
  inline static int8_t between(char * const &a,const char * const &b)
  {
    int ans = strcmp(a,b);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(char * const &a,const char * const &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(char * const &a,const char * const &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(char * const &a,const char * const &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(char * const &a,const char * const &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(char * const &a,const char * const &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(char * const &a,const char * const &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
#if ARDUINO_UNIT_USE_FLASH  > 0
template  <  > struct Compare<char *,const __FlashStringHelper *>
{
  inline static int8_t between(char * const &a,const __FlashStringHelper * const &b)
  {
    int ans = strcmp_P(a,(const char *)b);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(char * const &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(char * const &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(char * const &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(char * const &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(char * const &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(char * const &a,const __FlashStringHelper * const &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
#endif
template  <  > struct Compare<char *,char *>
{
  inline static int8_t between(char * const &a,char * const &b)
  {
    int ans = strcmp(a,b);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(char * const &a,char * const &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(char * const &a,char * const &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(char * const &a,char * const &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(char * const &a,char * const &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(char * const &a,char * const &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(char * const &a,char * const &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
template  < size_t M > struct Compare<char *,char [M]>
{
  inline static int8_t between(char * const &a,const char (&b)[M])
  {
    int ans = strcmp(a,b);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(char * const &a,const char (&b)[M])
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(char * const &a,const char (&b)[M])
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(char * const &a,const char (&b)[M])
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(char * const &a,const char (&b)[M])
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(char * const &a,const char (&b)[M])
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(char * const &a,const char (&b)[M])
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
template  < size_t N > struct Compare<char [N],String>
{
  inline static int8_t between(const char (&a)[N],const String &b)
  {
    int ans = -b.compareTo(a);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(const char (&a)[N],const String &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const char (&a)[N],const String &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const char (&a)[N],const String &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const char (&a)[N],const String &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const char (&a)[N],const String &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const char (&a)[N],const String &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
template  < size_t N > struct Compare<char [N],const char *>
{
  inline static int8_t between(const char (&a)[N],const char * const &b)
  {
    int ans = strcmp(a,b);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(const char (&a)[N],const char * const &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const char (&a)[N],const char * const &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const char (&a)[N],const char * const &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const char (&a)[N],const char * const &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const char (&a)[N],const char * const &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const char (&a)[N],const char * const &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
#if ARDUINO_UNIT_USE_FLASH  > 0
template  < size_t N > struct Compare<char [N],const __FlashStringHelper *>
{
  inline static int8_t between(const char (&a)[N],const __FlashStringHelper * const &b)
  {
    int ans = strcmp_P(a,(const char *)b);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(const char (&a)[N],const __FlashStringHelper * const &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const char (&a)[N],const __FlashStringHelper * const &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const char (&a)[N],const __FlashStringHelper * const &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const char (&a)[N],const __FlashStringHelper * const &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const char (&a)[N],const __FlashStringHelper * const &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const char (&a)[N],const __FlashStringHelper * const &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
#endif
template  < size_t N > struct Compare<char [N],char *>
{
  inline static int8_t between(const char (&a)[N],char * const &b)
  {
    int ans = strcmp(a,b);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(const char (&a)[N],char * const &b)
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const char (&a)[N],char * const &b)
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const char (&a)[N],char * const &b)
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const char (&a)[N],char * const &b)
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const char (&a)[N],char * const &b)
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const char (&a)[N],char * const &b)
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
template  < size_t N, size_t M > struct Compare<char [N],char [M]>
{
  inline static int8_t between(const char (&a)[N],const char (&b)[M])
  {
    int ans = strcmp(a,b);
    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;
  } // between
  inline static bool equal(const char (&a)[N],const char (&b)[M])
  {
    return between(a,b) == 0;
  } // equal
  inline static bool notEqual(const char (&a)[N],const char (&b)[M])
  {
    return between(a,b) != 0;
  } // notEqual
  inline static bool less(const char (&a)[N],const char (&b)[M])
  {
    return between(a,b) < 0;
  } // less
  inline static bool more(const char (&a)[N],const char (&b)[M])
  {
    return between(a,b) > 0;
  } // more
  inline static bool lessOrEqual(const char (&a)[N],const char (&b)[M])
  {
    return between(a,b) <= 0;
  } // lessOrEqual
  inline static bool moreOrEqual(const char (&a)[N],const char (&b)[M])
  {
    return between(a,b) >= 0;
  } // moreOrEqual
};
template <typename A, typename B> int8_t compareBetween(const A &a, const B &b) { return Compare<typename ArduinoUnitWiden<A>::type,typename ArduinoUnitWiden<B>::type>::between(a,b); }
template <typename A, typename B> bool compareEqual(const A &a, const B &b) { return Compare<typename ArduinoUnitWiden<A>::type,typename ArduinoUnitWiden<B>::type>::equal(a,b); }
template <typename A, typename B> bool compareNotEqual(const A &a, const B &b) { return Compare<typename ArduinoUnitWiden<A>::type,typename ArduinoUnitWiden<B>::type>::notEqual(a,b); }
template <typename A, typename B> bool compareLess(const A &a, const B &b) { return Compare<typename ArduinoUnitWiden<A>::type,typename ArduinoUnitWiden<B>::type>::less(a,b); }
template <typename A, typename B> bool compareMore(const A &a, const B &b) { return Compare<typename ArduinoUnitWiden<A>::type,typename ArduinoUnitWiden<B>::type>::more(a,b); }
template <typename A, typename B> bool compareLessOrEqual(const A &a, const B &b) { return Compare<typename ArduinoUnitWiden<A>::type,typename ArduinoUnitWiden<B>::type>::lessOrEqual(a,b); }
template <typename A, typename B> bool compareMoreOrEqual(const A &a, const B &b) { return Compare<typename ArduinoUnitWiden<A>::type,typename ArduinoUnitWiden<B>::type>::moreOrEqual(a,b); }

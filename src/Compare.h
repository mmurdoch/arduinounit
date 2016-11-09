#pragma once
#if defined(F)
#include <avr/pgmspace.h>
#endif
#include <WString.h>

template  < typename A, typename B > struct Compare
{
  inline static int between(const A &a,const B &b)
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
#if defined(F)
template  <  > struct Compare<String,const __FlashStringHelper *>;
#endif
template  <  > struct Compare<String,char *>;
template  < size_t M > struct Compare<String,char [M]>;
template  <  > struct Compare<const char *,String>;
template  <  > struct Compare<const char *,const char *>;
#if defined(F)
template  <  > struct Compare<const char *,const __FlashStringHelper *>;
#endif
template  <  > struct Compare<const char *,char *>;
template  < size_t M > struct Compare<const char *,char [M]>;
#if defined(F)
template  <  > struct Compare<const __FlashStringHelper *,String>;
#endif
#if defined(F)
template  <  > struct Compare<const __FlashStringHelper *,const char *>;
#endif
#if defined(F)
template  <  > struct Compare<const __FlashStringHelper *,const __FlashStringHelper *>;
#endif
#if defined(F)
template  <  > struct Compare<const __FlashStringHelper *,char *>;
#endif
#if defined(F)
template  < size_t M > struct Compare<const __FlashStringHelper *,char [M]>;
#endif
template  <  > struct Compare<char *,String>;
template  <  > struct Compare<char *,const char *>;
#if defined(F)
template  <  > struct Compare<char *,const __FlashStringHelper *>;
#endif
template  <  > struct Compare<char *,char *>;
template  < size_t M > struct Compare<char *,char [M]>;
template  < size_t N > struct Compare<char [N],String>;
template  < size_t N > struct Compare<char [N],const char *>;
#if defined(F)
template  < size_t N > struct Compare<char [N],const __FlashStringHelper *>;
#endif
template  < size_t N > struct Compare<char [N],char *>;
template  < size_t N, size_t M > struct Compare<char [N],char [M]>;
template  <  > struct Compare<String,String>
{
  inline static int between(const String &a,const String &b)
  {
    return a.compareTo(b);
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
  inline static int between(const String &a,const char * const &b)
  {
    return a.compareTo(b);
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
#if defined(F)
template  <  > struct Compare<String,const __FlashStringHelper *>
{
  inline static int between(const String &a,const __FlashStringHelper * const &b)
  {
    uint8_t a_buf[4],b_buf[4];
    uint16_t i=0;

    for (;;) {
      uint8_t j=(i%4);
      if (j == 0) {
         a.getBytes(a_buf,4,i);
         memcpy_P(b_buf,((const char *)b)+i,4);
      }
      if (a_buf[j] < b_buf[j]) return -1;
      if (a_buf[j] > b_buf[j]) return  1;
      if (a_buf[j] == 0) return 0;
      ++i;
    }
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
  inline static int between(const String &a,char * const &b)
  {
    return a.compareTo(b);
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
  inline static int between(const String &a,const char (&b)[M])
  {
    return a.compareTo(b);
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
  inline static int between(const char * const &a,const String &b)
  {
    return -b.compareTo(a);
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
  inline static int between(const char * const &a,const char * const &b)
  {
    return strcmp(a,b);
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
#if defined(F)
template  <  > struct Compare<const char *,const __FlashStringHelper *>
{
  inline static int between(const char * const &a,const __FlashStringHelper * const &b)
  {
    return strcmp_P(a,(const char *)b);
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
  inline static int between(const char * const &a,char * const &b)
  {
    return strcmp(a,b);
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
  inline static int between(const char * const &a,const char (&b)[M])
  {
    return strcmp(a,b);
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
#if defined(F)
template  <  > struct Compare<const __FlashStringHelper *,String>
{
  inline static int between(const __FlashStringHelper * const &a,const String &b)
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
#if defined(F)
template  <  > struct Compare<const __FlashStringHelper *,const char *>
{
  inline static int between(const __FlashStringHelper * const &a,const char * const &b)
  {
    return -strcmp_P(b,(const char *)a);
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
#if defined(F)
template  <  > struct Compare<const __FlashStringHelper *,const __FlashStringHelper *>
{
  inline static int between(const __FlashStringHelper * const &a,const __FlashStringHelper * const &b)
  {
    uint8_t a_buf[4],b_buf[4];
    uint16_t i=0;

    for (;;) {
      uint8_t j=(i%4);
      if (j == 0) {
         memcpy_P(a_buf,((const char *)a)+i,4);
         memcpy_P(b_buf,((const char *)b)+i,4);
      }
      if (a_buf[j] < b_buf[j]) return -1;
      if (a_buf[j] > b_buf[j]) return  1;
      if (a_buf[j] == 0) return 0;
      ++i;
    }
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
#if defined(F)
template  <  > struct Compare<const __FlashStringHelper *,char *>
{
  inline static int between(const __FlashStringHelper * const &a,char * const &b)
  {
    return -strcmp_P(b,(const char *)a);
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
#if defined(F)
template  < size_t M > struct Compare<const __FlashStringHelper *,char [M]>
{
  inline static int between(const __FlashStringHelper * const &a,const char (&b)[M])
  {
    return -strcmp_P(b,(const char *)a);
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
  inline static int between(char * const &a,const String &b)
  {
    return -b.compareTo(a);
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
  inline static int between(char * const &a,const char * const &b)
  {
    return strcmp(a,b);
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
#if defined(F)
template  <  > struct Compare<char *,const __FlashStringHelper *>
{
  inline static int between(char * const &a,const __FlashStringHelper * const &b)
  {
    return strcmp_P(a,(const char *)b);
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
  inline static int between(char * const &a,char * const &b)
  {
    return strcmp(a,b);
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
  inline static int between(char * const &a,const char (&b)[M])
  {
    return strcmp(a,b);
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
  inline static int between(const char (&a)[N],const String &b)
  {
    return -b.compareTo(a);
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
  inline static int between(const char (&a)[N],const char * const &b)
  {
    return strcmp(a,b);
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
#if defined(F)
template  < size_t N > struct Compare<char [N],const __FlashStringHelper *>
{
  inline static int between(const char (&a)[N],const __FlashStringHelper * const &b)
  {
    return strcmp_P(a,(const char *)b);
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
  inline static int between(const char (&a)[N],char * const &b)
  {
    return strcmp(a,b);
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
  inline static int between(const char (&a)[N],const char (&b)[M])
  {
    return strcmp(a,b);
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
template <typename A, typename B> int compareBetween(const A &a, const B &b) { return Compare<A,B>::between(a,b); }
template <typename A, typename B> bool compareEqual(const A &a, const B &b) { return Compare<A,B>::equal(a,b); }
template <typename A, typename B> bool compareNotEqual(const A &a, const B &b) { return Compare<A,B>::notEqual(a,b); }
template <typename A, typename B> bool compareLess(const A &a, const B &b) { return Compare<A,B>::less(a,b); }
template <typename A, typename B> bool compareMore(const A &a, const B &b) { return Compare<A,B>::more(a,b); }
template <typename A, typename B> bool compareLessOrEqual(const A &a, const B &b) { return Compare<A,B>::lessOrEqual(a,b); }
template <typename A, typename B> bool compareMoreOrEqual(const A &a, const B &b) { return Compare<A,B>::moreOrEqual(a,b); }

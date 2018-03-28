#pragma once
#include <stdint.h>
#include "ArduinoUnitWiden.h"

template  < typename A, typename B > struct Compare
{
  inline static int8_t between(const A &a,const B &b)
  {
    return (a<b) ? -1 : (b<a) ? 1 : 0;
  } // between
  inline static bool equal(const A &a,const B &b)
  {
    return !(less(a,b) || Compare<B,A>::less(b,a));
  } // equal
  inline static bool notEqual(const A &a,const B &b)
  {
    return !equal(a,b);
  } // notEqual
  inline static bool less(const A &a,const B &b)
  {
    return a<b;
  } // less
  inline static bool more(const A &a,const B &b)
  {
    return Compare<B,A>::less(b,a);
  } // more
  inline static bool lessOrEqual(const A &a,const B &b)
  {
    return !Compare<B,A>::less(b,a);
  } // lessOrEqual
  inline static bool moreOrEqual(const A &a,const B &b)
  {
    return !less(a,b);
  } // moreOrEqual
};

template <typename A, typename B> int8_t compareBetween(const A &a, const B &b) { return Compare<typename ArduinoUnitWiden<A>::type,typename ArduinoUnitWiden<B>::type>::between(a,b); }
template <typename A, typename B> bool compareEqual(const A &a, const B &b) { return Compare<typename ArduinoUnitWiden<A>::type,typename ArduinoUnitWiden<B>::type>::equal(a,b); }
template <typename A, typename B> bool compareNotEqual(const A &a, const B &b) { return Compare<typename ArduinoUnitWiden<A>::type,typename ArduinoUnitWiden<B>::type>::notEqual(a,b); }
template <typename A, typename B> bool compareLess(const A &a, const B &b) { return Compare<typename ArduinoUnitWiden<A>::type,typename ArduinoUnitWiden<B>::type>::less(a,b); }
template <typename A, typename B> bool compareMore(const A &a, const B &b) { return Compare<typename ArduinoUnitWiden<A>::type,typename ArduinoUnitWiden<B>::type>::more(a,b); }
template <typename A, typename B> bool compareLessOrEqual(const A &a, const B &b) { return Compare<typename ArduinoUnitWiden<A>::type,typename ArduinoUnitWiden<B>::type>::lessOrEqual(a,b); }
template <typename A, typename B> bool compareMoreOrEqual(const A &a, const B &b) { return Compare<typename ArduinoUnitWiden<A>::type,typename ArduinoUnitWiden<B>::type>::moreOrEqual(a,b); }

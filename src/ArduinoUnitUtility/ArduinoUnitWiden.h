#pragma once

#include "ArduinoUnitMockWString.h"

class ArduinoUnitString;

// these are used to narrow the number of specializations the templates generate
template< typename T > struct ArduinoUnitRemoveReferenceTemplate      {typedef T type;};
template< typename T > struct ArduinoUnitRemoveReferenceTemplate<T&>  {typedef T type;};
template< typename T > struct ArduinoUnitRemoveReferenceTemplate<T&&> {typedef T type;};
template< typename T > struct ArduinoUnitRemoveConstTemplate      {typedef T type;};
template< typename T > struct ArduinoUnitRemoveConstTemplate<const T>  {typedef T type;};
template< typename T > struct ArduinoUnitRemoveConstTemplate<const T *>  {typedef T* type;};
template< typename T > struct ArduinoUnitRemoveConstTemplate<const T []>  {typedef T type[];};
template< typename T, int N > struct ArduinoUnitRemoveConstTemplate<const T[N]>  {typedef T type[N];};

// these (safely) widen the types in assertions to reduce the number of specializations generated
template< typename T > struct ArduinoUnitWiden { typedef T type; };
template< > struct ArduinoUnitWiden < float > { typedef double type; };
template< > struct ArduinoUnitWiden < __FlashStringHelper * > { typedef ArduinoUnitString type; };
template< > struct ArduinoUnitWiden < String > { typedef ArduinoUnitString type; };
#if ! defined(ARDUINO)
template< > struct ArduinoUnitWiden < std::string > { typedef ArduinoUnitString type; };
#endif
template< > struct ArduinoUnitWiden < char * > { typedef ArduinoUnitString type; };
template< > struct ArduinoUnitWiden < char [] > { typedef ArduinoUnitString type; };
template<int N> struct ArduinoUnitWiden < char [N] > { typedef ArduinoUnitString type; };

//
// Welcome to the Arduino Unit trailer park...  This is the same idea
// as Widen, but takes both argument types into account.
//
template <typename TA, typename TB> struct ArduinoUnitDoubleWide {
  typedef TA A;
  typedef TB B;
};

// signed types

template < > struct ArduinoUnitDoubleWide < char , char > {
  typedef int A;
  typedef int B;
};

template < > struct ArduinoUnitDoubleWide < char , short > {
  typedef int A;
  typedef int B;
};

template < > struct ArduinoUnitDoubleWide < char , int > {
  typedef int A;
  typedef int B;
};

template < > struct ArduinoUnitDoubleWide < char , long > {
  typedef long A;
  typedef long B;
};

template < > struct ArduinoUnitDoubleWide < char , long long > {
  typedef long long A;
  typedef long long B;
};

template < > struct ArduinoUnitDoubleWide < short , char > {
  typedef int A;
  typedef int B;
};

template < > struct ArduinoUnitDoubleWide < short , short > {
  typedef int A;
  typedef int B;
};

template < > struct ArduinoUnitDoubleWide < short , int > {
  typedef int A;
  typedef int B;
};

template < > struct ArduinoUnitDoubleWide < short , long > {
  typedef long A;
  typedef long B;
};

template < > struct ArduinoUnitDoubleWide < short , long long > {
  typedef long long A;
  typedef long long B;
};

template < > struct ArduinoUnitDoubleWide < int , char > {
  typedef int A;
  typedef int B;
};

template < > struct ArduinoUnitDoubleWide < int , short > {
  typedef int A;
  typedef int B;
};

template < > struct ArduinoUnitDoubleWide < int , int > {
  typedef int A;
  typedef int B;
};

template < > struct ArduinoUnitDoubleWide < int , long > {
  typedef long A;
  typedef long B;
};

template < > struct ArduinoUnitDoubleWide < int , long long > {
  typedef long long A;
  typedef long long B;
};

template < > struct ArduinoUnitDoubleWide < long , char > {
  typedef long A;
  typedef long B;
};

template < > struct ArduinoUnitDoubleWide < long , short > {
  typedef long A;
  typedef long B;
};

template < > struct ArduinoUnitDoubleWide < long , int > {
  typedef long A;
  typedef long B;
};

template < > struct ArduinoUnitDoubleWide < long , long > {
  typedef long A;
  typedef long B;
};

template < > struct ArduinoUnitDoubleWide < long , long long > {
  typedef long long A;
  typedef long long B;
};

template < > struct ArduinoUnitDoubleWide < long long , char > {
  typedef long long A;
  typedef long long B;
};

template < > struct ArduinoUnitDoubleWide < long long , short > {
  typedef long long A;
  typedef long long B;
};

template < > struct ArduinoUnitDoubleWide < long long , int > {
  typedef long long A;
  typedef long long B;
};

template < > struct ArduinoUnitDoubleWide < long long , long > {
  typedef long long A;
  typedef long long B;
};

template < > struct ArduinoUnitDoubleWide < long long , long long > {
  typedef long long A;
  typedef long long B;
};


// unsigned types

template < > struct ArduinoUnitDoubleWide < unsigned char , unsigned char > {
  typedef unsigned int A;
  typedef unsigned int B;
};

template < > struct ArduinoUnitDoubleWide < unsigned char , unsigned short > {
  typedef unsigned int A;
  typedef unsigned int B;
};

template < > struct ArduinoUnitDoubleWide < unsigned char , unsigned int > {
  typedef unsigned int A;
  typedef unsigned int B;
};

template < > struct ArduinoUnitDoubleWide < unsigned char , unsigned long > {
  typedef unsigned long A;
  typedef unsigned long B;
};

template < > struct ArduinoUnitDoubleWide < unsigned char , unsigned long long > {
  typedef unsigned long long A;
  typedef unsigned long long B;
};

template < > struct ArduinoUnitDoubleWide < unsigned short , unsigned char > {
  typedef unsigned int A;
  typedef unsigned int B;
};

template < > struct ArduinoUnitDoubleWide < unsigned short , unsigned short > {
  typedef unsigned int A;
  typedef unsigned int B;
};

template < > struct ArduinoUnitDoubleWide < unsigned short , unsigned int > {
  typedef unsigned int A;
  typedef unsigned int B;
};

template < > struct ArduinoUnitDoubleWide < unsigned short , unsigned long > {
  typedef unsigned long A;
  typedef unsigned long B;
};

template < > struct ArduinoUnitDoubleWide < unsigned short , unsigned long long > {
  typedef unsigned long long A;
  typedef unsigned long long B;
};

template < > struct ArduinoUnitDoubleWide < unsigned int , unsigned char > {
  typedef unsigned int A;
  typedef unsigned int B;
};

template < > struct ArduinoUnitDoubleWide < unsigned int , unsigned short > {
  typedef unsigned int A;
  typedef unsigned int B;
};

template < > struct ArduinoUnitDoubleWide < unsigned int , unsigned int > {
  typedef unsigned int A;
  typedef unsigned int B;
};

template < > struct ArduinoUnitDoubleWide < unsigned int , unsigned long > {
  typedef unsigned long A;
  typedef unsigned long B;
};

template < > struct ArduinoUnitDoubleWide < unsigned int , unsigned long long > {
  typedef unsigned long long A;
  typedef unsigned long long B;
};

template < > struct ArduinoUnitDoubleWide < unsigned long , unsigned char > {
  typedef unsigned long A;
  typedef unsigned long B;
};

template < > struct ArduinoUnitDoubleWide < unsigned long , unsigned short > {
  typedef unsigned long A;
  typedef unsigned long B;
};

template < > struct ArduinoUnitDoubleWide < unsigned long , unsigned int > {
  typedef unsigned long A;
  typedef unsigned long B;
};

template < > struct ArduinoUnitDoubleWide < unsigned long , unsigned long > {
  typedef unsigned long A;
  typedef unsigned long B;
};

template < > struct ArduinoUnitDoubleWide < unsigned long , unsigned long long > {
  typedef unsigned long long A;
  typedef unsigned long long B;
};

template < > struct ArduinoUnitDoubleWide < unsigned long long , unsigned char > {
  typedef unsigned long long A;
  typedef unsigned long long B;
};

template < > struct ArduinoUnitDoubleWide < unsigned long long , unsigned short > {
  typedef unsigned long long A;
  typedef unsigned long long B;
};

template < > struct ArduinoUnitDoubleWide < unsigned long long , unsigned int > {
  typedef unsigned long long A;
  typedef unsigned long long B;
};

template < > struct ArduinoUnitDoubleWide < unsigned long long , unsigned long > {
  typedef unsigned long long A;
  typedef unsigned long long B;
};

template < > struct ArduinoUnitDoubleWide < unsigned long long , unsigned long long > {
  typedef unsigned long long A;
  typedef unsigned long long B;
};

#define ArduinoUnitType(T) ArduinoUnitWiden < ArduinoUnitRemoveConstTemplate < ArduinoUnitRemoveReferenceTemplate < T >::type >::type >::type

#if __cplusplus < 201103L
#  define ArduinoUnitArgType(X) ArduinoUnitType(__typeof__(X))
#else
#  define ArduinoUnitArgType(X) ArduinoUnitType(decltype(X))
#endif

#define ArduinoUnitArgTypes(X,Y) ArduinoUnitDoubleWide < ArduinoUnitArgType(X) , ArduinoUnitArgType(Y) >

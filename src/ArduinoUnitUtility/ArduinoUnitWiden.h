#pragma once

#if defined(ARDUINO)
#include "WString.h"
#else
#include <string>
#endif

#include "Flash.h"
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
template< > struct ArduinoUnitWiden < char > { typedef int type; };
template< > struct ArduinoUnitWiden < unsigned char > { typedef unsigned int type; };
template< > struct ArduinoUnitWiden < short > { typedef int type; };
template< > struct ArduinoUnitWiden < unsigned short > { typedef unsigned int type; };
template< > struct ArduinoUnitWiden < float > { typedef double type; };
#if ARDUINO_UNIT_USE_FLASH > 0
template< > struct ArduinoUnitWiden < __FlashStringHelper * > { typedef ArduinoUnitString type; };
#endif
#if defined(ARDUINO)
template< > struct ArduinoUnitWiden < String > { typedef ArduinoUnitString type; };
#else
template< > struct ArduinoUnitWiden < std::string > { typedef ArduinoUnitString type; };
#endif
template< > struct ArduinoUnitWiden < char * > { typedef ArduinoUnitString type; };
template< > struct ArduinoUnitWiden < char [] > { typedef ArduinoUnitString type; };
template<int N> struct ArduinoUnitWiden < char [N] > { typedef ArduinoUnitString type; };

#define ArduinoUnitType(T) ArduinoUnitWiden < ArduinoUnitRemoveConstTemplate < ArduinoUnitRemoveReferenceTemplate < T >::type >::type >::type
#define ArduinoUnitArgType(X) ArduinoUnitType(__typeof__(X))


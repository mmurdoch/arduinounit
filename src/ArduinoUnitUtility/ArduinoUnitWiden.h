#pragma once

// these are used to narrow the number of specializations the templates generate
template< typename T > struct ArduinoUnitRemoveReferenceTemplate      {typedef T type;};
template< typename T > struct ArduinoUnitRemoveReferenceTemplate<T&>  {typedef T type;};
template< typename T > struct ArduinoUnitRemoveReferenceTemplate<T&&> {typedef T type;};
template< typename T > struct ArduinoUnitRemoveConstTemplate      {typedef T type;};
template< typename T > struct ArduinoUnitRemoveConstTemplate<const T>  {typedef T type;};

// these (safely) widen the types in assertions to reduce the number of specializations generated
template< typename T > struct ArduinoUnitWiden { typedef T type; };
template< > struct ArduinoUnitWiden < char > { typedef int type; };
template< > struct ArduinoUnitWiden < unsigned char > { typedef unsigned int type; };
template< > struct ArduinoUnitWiden < short > { typedef int type; };
template< > struct ArduinoUnitWiden < unsigned short > { typedef unsigned int type; };
template< > struct ArduinoUnitWiden < float > { typedef double type; };
template< > struct ArduinoUnitWiden < char * > { typedef const char *type; };
template< > struct ArduinoUnitWiden < char [] > { typedef const char *type; };
template<int N> struct ArduinoUnitWiden < char [N] > { typedef const char *type; };
template<int N> struct ArduinoUnitWiden < const char [N] > { typedef const char *type; };
template< > struct ArduinoUnitWiden < const char [] > { typedef const char *type; };

#define ArduinoUnitType(T) ArduinoUnitWiden < ArduinoUnitRemoveConstTemplate < ArduinoUnitRemoveReferenceTemplate < T >::type >::type >::type
#define ArduinoUnitArgType(X) ArduinoUnitType(__typeof__(X))

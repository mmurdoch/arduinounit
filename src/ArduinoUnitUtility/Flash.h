#pragma once

//
// find progmem, and setup flash string de-duplication for avrs...
//
#if defined(F)
#  if defined(__AVR__)
//
//   http://michael-buschbeck.github.io/arduino/2013/10/22/string-merging-pstr-percent-codes/
//
// This additionally uses __COUNTER__ for generating unique labels,
// since backreferencing a fixed label 0 as suggested in the above url
// breaks (probably in recent compiler version, probably because it
// reorders asm blocks).

#    define ARDUINO_UNIT_PSTR2(str, counter) \
       (__extension__({ \
         PGM_P ptr;  \
         asm volatile \
         ( \
           ".pushsection .progmem.mergeable-strings, \"SM\", @progbits, 1" "\n\t" \
           #counter ": .string " #str                                      "\n\t" \
           ".popsection"                                                   "\n\t" \
         ); \
         asm volatile \
         ( \
           "ldi %A0, lo8(" #counter "b)"                                   "\n\t" \
           "ldi %B0, hi8(" #counter "b)"                                   "\n\t" \
           : "=d" (ptr) \
         ); \
         ptr; \
       }))
// Use a three-layer macro to ensure that __COUNTER__ is expanded before
// stringification above.
#    define ARDUINO_UNIT_PSTR1(str, counter) ARDUINO_UNIT_PSTR2(str, counter)
#    define ARDUINO_UNIT_PSTR(str) ARDUINO_UNIT_PSTR1(str, __COUNTER__)
#    define ARDUINO_UNIT_STRING(STR) (reinterpret_cast<const __FlashStringHelper *>(ARDUINO_UNIT_PSTR(STR)))
#    define ARDUINO_UNIT_DECLARE_STRING const __FlashStringHelper *
#    define ARDUINO_UNIT_USE_FLASH 1
#  endif
#endif

#if !defined(ARDUINO_UNIT_STRING)
#  define ARDUINO_UNIT_STRING(STR) ((const char *)(STR))
#  define ARDUINO_UNIT_DECLARE_STRING const char *
#  if defined(PROGMEM)
#    define ARDUINO_UNIT_USE_FLASH 1
#  else
#    define ARDUINO_UNIT_USE_FLASH 0
#  endif
#endif

class __FlashStringHelper;
#ifdef F
#undef F
#endif

//#define F(stringLiteral) (([]()->const __FlashStringHelper * { return (const __FlashStringHelper *) ARDUINO_UNIT_STRING(stringLiteral); })())
#define F(stringLiteral) ((const __FlashStringHelper *) ARDUINO_UNIT_STRING(stringLiteral))

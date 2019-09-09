#pragma once

//
// find progmem, and setup flash string de-duplication for avrs...
//
#if defined(F)
#  if defined(__AVR__)
//
//   http://michael-buschbeck.github.io/arduino/2013/10/22/string-merging-pstr-percent-codes/
//
#    define ARDUINO_UNIT_PSTR(str) \
       (__extension__({ \
         PGM_P ptr ## __COUNTER__;  \
         asm volatile \
         ( \
           ".pushsection .progmem.mergeable-strings, \"SM\", @progbits, 1" "\n\t" \
           "0: .string " #str                                              "\n\t" \
           ".popsection"                                                   "\n\t" \
         ); \
         asm volatile \
         ( \
           "ldi %A0, lo8(0b)"                                              "\n\t" \
           "ldi %B0, hi8(0b)"                                              "\n\t" \
           : "=d" (ptr ## __COUNTER__) \
         ); \
         ptr ## __COUNTER__ ; \
       }))
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

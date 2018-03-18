#pragma once

//
// find progmem, and setup flash string de-duplication for avrs...
//
#if defined(F)
#  if defined(ESP8266)
#    include <cores/esp8266/pgmspace.h>
#  elif defined(ESP32)
#    include <cores/esp32/pgmspace.h>
#  else
#    include <avr/pgmspace.h>
//
//   http://michael-buschbeck.github.io/arduino/2013/10/22/string-merging-pstr-percent-codes/
//
#    undef PSTR
#    define PSTR(str) \
       (__extension__({ \
         PGM_P ptr;  \
         asm volatile \
         ( \
           ".pushsection .progmem.data, \"SM\", @progbits, 1" "\n\t" \
           "0: .string " #str                                 "\n\t" \
           ".popsection"                                      "\n\t" \
         ); \
         asm volatile \
         ( \
           "ldi %A0, lo8(0b)"                                 "\n\t" \
           "ldi %B0, hi8(0b)"                                 "\n\t" \
           : "=d" (ptr) \
         ); \
         ptr; \
       }))
#  endif
#endif

//
// Workaround for Arduino Due
//
#if defined(__arm__) && !defined(PROGMEM)
#define PSTR(s) s
#define memcpy_P(a, b, c) memcpy(a, b, c)
#define strlen_P(a) strlen(a)
#endif

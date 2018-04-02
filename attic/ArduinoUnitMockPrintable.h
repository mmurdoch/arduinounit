#pragma once

#include <stddef.h>
#include <stdint.h>

#if defined(ARDUINO)
#include <Printable.h>
#else

class Print;
class Printable;

class Printable {
 public: virtual size_t printTo(Print& p) const = 0;
 public: ~Printable();
};

#endif

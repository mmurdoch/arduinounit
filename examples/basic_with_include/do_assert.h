#line 2 "do_assert.h"
#include <ArduinoUnit.h>

//
// IMPORTANT: Create a test context to perform asserts in the context of tests declared elsewhere.
//
testcontext();

void performAssertion() {
  int y=1;
  assertNotEqual(y,1);
}


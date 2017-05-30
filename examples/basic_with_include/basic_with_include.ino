#line 2 "basic.ino"
#include <ArduinoUnit.h>
#include "do_assert.h"

test(correct)
{
  int x=1;
  assertEqual(x,1);
}

test(incorrect)
{
  int x=1;
  assertNotEqual(x,1);
}

test(incorrect_included)
{
  performAssertion();  // included 
}

void setup()
{
  Serial.begin(9600);
  while(!Serial); // for the Arduino Leonardo/Micro only
}

void loop()
{
  Test::run();
}

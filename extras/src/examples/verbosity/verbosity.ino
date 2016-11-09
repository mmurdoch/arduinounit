#line 2 "verbosity.ino"
#include <ArduinoUnit.h>

test(loud_pass)
{
  verbosity = TEST_VERBOSITY_ALL;
  int x=1;
  assertEqual(x,1);
}

test(loud_fail)
{
  verbosity = TEST_VERBOSITY_ALL;
  int x=1;
  assertNotEqual(x,1);
}

test(loud_skip)
{
  verbosity = TEST_VERBOSITY_ALL;
  skip();
}

test(quiet_pass)
{
  verbosity = TEST_VERBOSITY_NONE;
  int x=1;
  assertEqual(x,1);
}

test(quiet_fail)
{
  verbosity = TEST_VERBOSITY_NONE;
  int x=1;
  assertNotEqual(x,1);
}

test(quiet_skip)
{
  verbosity = TEST_VERBOSITY_NONE;
  skip();
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

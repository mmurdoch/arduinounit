#line 2 "basic.ino"
//#define TEST_REDUCE_CODE_FOOTPRINT 1
#include <ArduinoUnit.h>

test(correct)
{
  int x=1;
  assertEqual(x,1);
}

test(incorrect)
{
  int x=1;
  assertNotEqual(x,1);
  assertNotEqual(x,1);
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

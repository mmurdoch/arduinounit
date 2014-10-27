#line 2 "continuous.ino"
#include <ArduinoUnit.h>

// test-once test named "once"
test(once)
{
  int x=1;
  assertLessOrEqual(x,1);
}

// test-until-skip-pass-or-fail test named "continuous"
testing(continuous)
{
  unsigned long t = millis();
  assertLessOrEqual(t,100);
  if (t >= 100) pass();
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

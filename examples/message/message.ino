#line 2 "basic.ino"
#include <ArduinoUnit.h>

test(correct)
{
  int x=1;
  // Use F() to store a string in flash, which saves RAM for long strings
  assertEqual(x,1,F("inconceivable!"));
}

test(incorrect)
{
  int x=1;
  // Compose a message with A << B << C << ...
  assertNotEqual(x,1,F("we are saying x != 1, which is false when x=") << x << F(", see?"));
}

test(cases)
{
  int x=3;
  for (int k=0; k<4; ++k) {
    assertNotEqual(x,2*k-1,"case k=" << k);
  }
}

void setup()
{
  Serial.begin(9600);
  while(!Serial) {} // Portability for Leonardo/Micro
}

void loop()
{
  Test::run();
}

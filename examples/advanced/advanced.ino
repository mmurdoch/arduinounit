#line 2 "advanced.ino"
#include <ArduinoUnit.h>

test(simple1)
{
  assertTrue(true);
}

test(simple2)
{
  assertTrue(false);
}

class MyTestOnce : public TestOnce
{
public:
  // constructor must name test

  MyTestOnce(const char *name) 
  : TestOnce(name) 
  {
    // lightweight constructor, since
    // this test might be skipped.
    // you can adjust verbosity here

    verbosity = TEST_VERBOSITY_ALL;
  }
  
  int n;

  void setup()
  {
    n = random(6);
    if (n == 0) skip();
  }

  void once()
  {
    for (int i=-n; i<=n; ++i) {
      for (int j=-n; j<=n; ++j) {
        assertEqual(i+j,j+i);
      }
    }
  }
};

MyTestOnce myTestOnce1("myTestOnce1");
MyTestOnce myTestOnce2("myTestOnce2");
MyTestOnce myTestOnce3("myTestOnce3");

class MyTest : public Test
{
public:
  uint16_t when;
  MyTest(const char *name) 
  : Test(name)
  {
    when = random(100,200);
  }

  void loop()
  {
    if (millis() >= when) 
    {
      assertLess(random(100),50);
      pass(); // if assertion is ok
    }
  }  
};

MyTest myTest1("myTest1");
MyTest myTest2("myTest2");
MyTest myTest3("myTest3");

void setup()
{
  Serial.begin(9600);
  while(!Serial); // for the Arduino Leonardo/Micro only

  Test::min_verbosity |= TEST_VERBOSITY_ASSERTIONS_ALL;
  Test::exclude("my*2");
}

void loop()
{
  Test::run();
}

#include "ArduinoUnit.h"

#include "ArduinoUnitUtility/ArduinoUnitMockWString.h"

test(stringCons) {
  String hw1("hello world");
  String hw2(F("hello world"));
  String hw3(hw1);

  assertEqual(hw1,hw2);
  assertEqual(hw2,hw3);
  assertEqual(hw3,hw1);    
}

test(stringVals) {
  for (int i=0; i<10; ++i) {
    int x = rand();
    String s(x);
    char tmp[32];
    snprintf(tmp,sizeof(tmp),"%d",x);
    assertEqual(s,tmp);
  }
}

int main() {
  Test::min_verbosity = TEST_VERBOSITY_ALL;
  while (!Test::finished()) {
    Test::run();
  }
  return 0;
}

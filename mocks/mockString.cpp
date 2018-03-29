#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "ArduinoUnitUtility/ArduinoUnitMockWString.h"

void testStringCons() {
  String hw1("hello world");
  String hw2(F("hello world"));
  String hw3(hw1);

  assert(strcmp(hw1.c_str(),"hello world")==0);
  assert(strcmp(hw2.c_str(),"hello world")==0);
  assert(strcmp(hw3.c_str(),"hello world")==0);  
}

void testStringNums() {
  String s1;
  s1 += 42;
  assert(strcmp(s1.c_str(),"42")==0);

  String s2;
  s2 += 42.1;
  printf("s='%s'\n",s2.c_str());
  assert(strcmp(s2.c_str(),"42.10")==0);  
}

int main() {
  testStringCons();
  testStringNums();  
  printf("ok\n");
  return 0;
}

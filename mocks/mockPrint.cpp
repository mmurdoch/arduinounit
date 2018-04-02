#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <assert.h>
#include "ArduinoUnitUtility/ArduinoUnitMock2WString.h"
#include "ArduinoUnitUtility/ArduinoUnitMock2Print.h"

struct StringPrint : Print, String {
  size_t write(uint8_t c) {
    uint8_t tmp[1];
    tmp[0]=c;
    return write(tmp,1);
  }
  size_t write(const uint8_t *buffer, size_t size) {
    concat((const char*)buffer,size);
    return size;
  }
};

void testPrint() {
  StringPrint sp;
  sp.print("hello");
  sp.print(' ');
  sp.print("world");
  sp.println("!");  
  assert(strcmp(sp.c_str(),"hello world!\r\n")==0);
}

void testCppStreamPrint() {
  std::ostringstream oss;
  CppStreamPrint sp(oss);
  sp.println("check 1");
  sp.println("check 2");
  std::string ans = oss.str();
  assert(ans == "check 1\r\ncheck 2\r\n");
}

int main() {
  testPrint();
  testCppStreamPrint();
  printf("ok\n");
}


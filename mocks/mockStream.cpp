#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <assert.h>
#include "ArduinoUnitUtility/ArduinoUnitMockWString.h"
#include "ArduinoUnitUtility/ArduinoUnitMockPrint.h"
#include "ArduinoUnitUtility/ArduinoUnitMockStream.h"

std::string str(const char *s) { return s; }
std::string str(const std::string &s) { return s; }
std::string str(const String &s) { return s.c_str(); }

void err() {
  std::cout << "err" << std::endl;
}
#define ASSERT_SEQ(A,B) { std::string a=str(A); std::string b=str(B); if (a != b) { std::cout << "(" << #A << "=='" << a << "')!=(" << #B << "=='" << b << "') on line " << __LINE__ << std::endl; err(); } }

#define ASSERT_IEQ(A,B) { int a=(A); int b=(B); if (a != b) { std::cout << "(" << #A << "==" << a << ")!=(" << #B << "==" << b << ") on line " << __LINE__ << std::endl; err(); } }

struct StringStream : Stream, String {
  size_t write(uint8_t c) {
    uint8_t tmp[1];
    tmp[0]=c;
    return write(tmp,1);
  }
  size_t write(const uint8_t *buffer, size_t size) {
    concat((const char*)buffer,size);
    return size;
  }

  int at;
  StringStream() { at = 0; }
  int available() { return length() - at; }
  virtual int read() {
    if (at < length()) {
      int ans = (*this)[at];
      ++at;
      return ans;
    } else {
      return -1;
    }
  }
  virtual int peek() {
    if (at < length()) {
      return (*this)[at];
    } else {
      return -1;
    }
  }
};

void testStream() {
  StringStream ss;

  ss.print("hello");
  ss.print(' ');
  ss.print("world");
  ss.println("!");
  ASSERT_SEQ(ss,"hello world!\r\n");

  char tmp[5];
  ss.readBytes(tmp,sizeof(tmp));
  assert(strncmp(tmp,"hello",sizeof(tmp))==0);  
  String w = ss.readString();
  ASSERT_SEQ(w," world!\r\n");
}

void testCppIOStream() {
  std::istringstream iss("test");  
  std::ostringstream oss;
  CppIOStream ios(iss,oss);
  ios.println("check 1");
  ios.println("check 2");
  String test = ios.readString();
  assert(oss.str() == "check 1\r\ncheck 2\r\n");
  assert(test == "test");
}

int main() {
  testStream();
  testCppIOStream();
  printf("ok\n");
}

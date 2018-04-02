#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <limits.h>
#include "ArduinoUnitUtility/ArduinoUnitMockWString.h"
#include "ArduinoUnitUtility/ArduinoUnitMockPrint.h"

std::string str(const char *s) { return s; }
std::string str(const std::string &s) { return s; }
std::string str(const String &s) { return s.c_str(); }

void err() {
  std::cout << "err" << std::endl;
}
#define ASSERT_SEQ(A,B) { std::string a=str(A); std::string b=str(B); if (a != b) { std::cout << "(" << #A << "=='" << a << "')!=(" << #B << "=='" << b << "') on line " << __LINE__ << std::endl; err(); } }

#define ASSERT_IEQ(A,B) { int a=(A); int b=(B); if (a != b) { std::cout << "(" << #A << "==" << a << ")!=(" << #B << "==" << b << ") on line " << __LINE__ << std::endl; err(); } }

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
  int availableForWrite() { return INT_MAX;  }
};

void testPrint() {
  { StringPrint sp; ASSERT_SEQ(sp,""); }
  { StringPrint sp; sp.print("hello"); sp.print(' '); sp.print("world"); sp.println("!"); ASSERT_SEQ(sp,"hello world!\r\n"); }  
  { StringPrint sp; sp.write('x'); ASSERT_SEQ(sp,"x"); }
  { StringPrint sp; sp.write((const uint8_t *)"test",4); ASSERT_SEQ(sp,"test"); }
  { StringPrint sp; sp.print(F("test")); ASSERT_SEQ(sp,"test"); }
  { StringPrint sp; sp.print(String("test")); ASSERT_SEQ(sp,"test"); }
  { StringPrint sp; sp.print("test"); ASSERT_SEQ(sp,"test"); }
  { StringPrint sp; sp.print('x'); ASSERT_SEQ(sp,"x"); }
  { StringPrint sp; sp.print('x'); ASSERT_SEQ(sp,"x"); }
  { StringPrint sp; sp.print((unsigned char) 10); ASSERT_SEQ(sp,"10"); }
  { StringPrint sp; sp.print((unsigned char) 0x6b,HEX); ASSERT_SEQ(sp,"6b"); }
  { StringPrint sp; sp.print((int) 0); ASSERT_SEQ(sp,"0"); }  
  { StringPrint sp; sp.print((int) 123456); ASSERT_SEQ(sp,"123456"); }
  { StringPrint sp; sp.print((int) -123456); ASSERT_SEQ(sp,"-123456"); }  
  { StringPrint sp; sp.print((double) 123.456); ASSERT_SEQ(sp,"123.46"); }
  { StringPrint sp; sp.print((double) -123.456); ASSERT_SEQ(sp,"-123.46"); } 
  { StringPrint sp; sp.println(42); ASSERT_SEQ(sp,"42\r\n"); } 
  { StringPrint sp; sp.println(); ASSERT_SEQ(sp,"\r\n"); }
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


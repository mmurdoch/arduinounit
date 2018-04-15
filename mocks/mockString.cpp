#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <assert.h>
#include <string>
#include "ArduinoUnitUtility/ArduinoUnitMockWString.h"

std::string str(const char *s) { return s; }
std::string str(const std::string &s) { return s; }
std::string str(const String &s) { return s.c_str(); }

void err() {
  std::cout << "err" << std::endl;
}
#define ASSERT_SEQ(A,B) { std::string a=str(A); std::string b=str(B); if (a != b) { std::cout << "(" << #A << "=='" << a << "')!=(" << #B << "=='" << b << "') on line " << __LINE__ << std::endl; err(); } }

#define ASSERT_IEQ(A,B) { int a=(A); int b=(B); if (a != b) { std::cout << "(" << #A << "==" << a << ")!=(" << #B << "==" << b << ") on line " << __LINE__ << std::endl; err(); } }

void testStringCons() {
  ASSERT_SEQ(String("hello"),"hello");
  ASSERT_SEQ(String(F("hello")),"hello");
  ASSERT_SEQ(String(String(F("hello"))),"hello");
  ASSERT_SEQ(String('x'),"x");
  ASSERT_SEQ(String((unsigned char) 10),"10");
  ASSERT_SEQ(String((unsigned char) 0x6b,16),"6b");
  ASSERT_SEQ(String(0),"0");
  ASSERT_SEQ(String(1),"1");
  ASSERT_SEQ(String(-1),"-1");      
  ASSERT_SEQ(String(-789),"-789");
  ASSERT_SEQ(String(3.14159),"3.14");
  ASSERT_SEQ(String(3.14159,4),"3.1416");

  ASSERT_IEQ(String("hi").length(),2);
  ASSERT_IEQ(String("").length(),0);

  ASSERT_SEQ(String("test")="me","me");
  ASSERT_SEQ(String("test")=F("me"),"me");
  ASSERT_SEQ(String("test")=String("me"),"me");    
  
  ASSERT_IEQ(String("aaron").compareTo(String("a"))>0,1);
  ASSERT_IEQ(String("aaron").compareTo(String("ab"))<0,1);
  ASSERT_IEQ(String("aaron").compareTo(String("aaron"))==0,1);

  ASSERT_IEQ(String("aaron").equalsIgnoreCase(String("aArOn")),1);
  ASSERT_IEQ(String("aaron").equalsIgnoreCase(String("aArOn0")),0);
  ASSERT_IEQ(String("aaron").startsWith(String("aro"),1),1);
  ASSERT_IEQ(String("aaron").startsWith(String("aro"),0),0);
  ASSERT_IEQ(String("aaron").endsWith(String("ron")),1);  
  
  ASSERT_IEQ(String("aaron").charAt(2),'r');
  ASSERT_IEQ(String("aaron")[2],'r');

  char tmp[32];
  String s("aaron");
  s.toCharArray(tmp,sizeof(tmp),1);
  ASSERT_SEQ(tmp,"aron");
  assert(s.end() == s.begin() + s.length());
  ASSERT_IEQ(s.indexOf('r'),2);
  ASSERT_IEQ(s.indexOf('x'),-1);
  ASSERT_IEQ(s.indexOf(String("on")),3);
  ASSERT_IEQ(s.lastIndexOf('a'),1);
  ASSERT_IEQ(s.lastIndexOf(String("ar")),1);

  ASSERT_SEQ(s.substring(2,4),"ro");

  s.replace('a','b');
  ASSERT_SEQ(s,"bbron");
  ASSERT_IEQ(String("bob").indexOf(String("ob")),1);

  s="bbrobbobb";
  s.remove(4,6);  
  ASSERT_SEQ(s,"bbroobb");
  s.remove(0,3);
  ASSERT_SEQ(s,"oobb");
  s.remove(2,5);
  ASSERT_SEQ(s,"oo");

  s="bbrobbobb";  
  s.replace(String("bb"),String("a"));
  ASSERT_SEQ(s,"aroaoa");
  s.replace(String("oa"),String("boa"));
  ASSERT_SEQ(s,"arboaboa");

  s="AaRon";
  s.toLowerCase();
  ASSERT_SEQ(s,"aaron");
  
  s="AaRon";
  s.toUpperCase();
  ASSERT_SEQ(s,"AARON");

  s="  AaRon   ";
  s.trim();
  ASSERT_SEQ(s,"AaRon");

  s="42";
  ASSERT_IEQ(s.toInt(),42);

  s="42.5";
  ASSERT_IEQ(s.toFloat()*2,85.0);

  s="42.5";
  ASSERT_IEQ(s.toDouble()*2,85.0);
}

void testStringNums() {
  String s1;
  s1 += 42;
  ASSERT_SEQ(s1,"42");

  String s2;
  s2 += 42.1;
  assert(strcmp(s2.c_str(),"42.10")==0);  
}

void testCppStrings() {
  ASSERT_SEQ(std::string("test"),String("test"));
}

int main() {
  testStringCons();
  testStringNums();
  testCppStrings();
  printf("ok\n");

  return 0;
}

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <assert.h>
#include "ArduinoUnitUtility/ArduinoUnitMockWString.h"
#include "ArduinoUnitUtility/ArduinoUnitMockPrint.h"
#include "ArduinoUnitUtility/ArduinoUnitMockStream.h"

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
  assert(strcmp(ss.c_str(),"hello world!\r\n")==0);
  char tmp[5];
  ss.readBytes(tmp,sizeof(tmp));
  assert(strncmp(tmp,"hello",sizeof(tmp))==0);  
  String w = ss.readString();
  assert(strcmp(w.c_str()," world!\r\n")==0);
}

struct CppIOStream : Stream {
  std::istream &in;
  std::ostream &out;
  std::string buffered;
  
  CppIOStream(std::istream &_in = std::cin, std::ostream &_out = std::cout) : in(_in), out(_out) {}

  int available() {
    if (buffered.size() < 32) {
      char tmp[32];
      size_t status = in.readsome(tmp,sizeof(tmp));
      if (status > 0) {
        buffered.append(tmp,status);
      }
    }
    return buffered.size();
  }

  virtual int read() {
    if (buffered.size() > 0) {
      int ans = buffered[0];
      buffered.erase(0,1);
      return ans;
    }
    return in.get();
  }
  
  virtual int peek() {
    return in.peek();
  }
  

  size_t write(uint8_t c) {
    char tmp[1];
    tmp[0]=c;
    out.write(tmp,1);
    return 1;
  }

  size_t write(const uint8_t *buffer, size_t size) {
    out.write((const char *)buffer,size);
    return size;
  }
};

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

#include "ArduinoUnitMockWString.h"

#if ! defined(ARDUINO)

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>

void String::concat(const char *str, unsigned int _len) {
  concatOk(str,_len);
}

unsigned char String::concatOk(const char *str, unsigned int _len) {
  if (capacity < len+_len || buffer == 0) {
    unsigned int _capacity = (len+_len)*5/4+4;
    char *_buffer = (char*) realloc(buffer, _capacity+1);
    if (_buffer == 0) {
      invalidate();
      return false;
    }

    // move str ref if it was moved in the realloc...
    if (buffer != 0 && str != 0 && buffer <= str && str <= buffer+capacity) {
      str = str + (_buffer - buffer);
    }
    capacity=_capacity;
    buffer=_buffer;
  }
  if (str != 0) {
    memmove(buffer+len,str,_len);
  } else {
    memset(buffer+len,0,_len);
  }
  len += _len;
  buffer[len]=0;
  return true;
}

unsigned char String::concat(long value, unsigned char base) {
  long x = value;
  int sz = 0;
  unsigned long p = 1;
  if (x < 0) {++sz; x = -x; p = p*10; }
  do { ++sz; x=x/base; } while (x != 0);
  unsigned at = len;
  if (concatOk((const char *) 0, sz)) {
    x=value;
    if (x < 0) { buffer[at++]='-'; x= -x; --sz; }
    at += sz;
    do {
      int d = x % base;
      char c = (d < 10) ? d+'0' : d+'a'-10;
      buffer[--at]=c;
      x = x/base;
    } while (x != 0);
    return true;
  } else {
    return false;
  }
}

unsigned char String::concat(unsigned long value, unsigned char base) {
  unsigned long x = value;
  int sz = 0;
  do { ++sz; x=x/base; } while (x != 0);
  unsigned at = len;
  if (concatOk((const char *) 0, sz)) {
    x=value;
    at += sz;
    do {
      int d = x % base;
      char c = (d < 10) ? d+'0' : d+'a'-10;
      buffer[--at]=c;
      x = x/base;
    } while (x != 0);
    return true;
  } else {
    return false;
  }
}

unsigned char String::concat(double value, unsigned char precision) {
  int sz = log10(fabs(value)+1)+1+precision+4;
  char tmp[sz];
  snprintf(tmp,sz,"%1.*lf",precision,value);
  return concatOk(tmp,strlen(tmp));
}

String::String(const char *_buffer, unsigned int _len)
  : buffer(0), capacity(0), len(0) {
  concat(_buffer,_len);
}

String::String(const char *str) : String(str,strlen(str)) {}

String::String(const String &str) : String(str.buffer,str.len) {}

String::String(const __FlashStringHelper *str) : String((const char *) str){}

String::String(char x) : String((const char *) 0,1) { buffer[0]=x; }

String::String(unsigned char x, unsigned char base)
  : buffer(0), capacity(0), len(0) {
  concat((unsigned long) x, base);
}

String::String(int x, unsigned char base)
  : buffer(0), capacity(0), len(0) {
  concat((long) x, base);
}

String::String(unsigned int x, unsigned char base)
  : buffer(0), capacity(0), len(0) {
  concat((unsigned long) x, base);
}

String::String(long x, unsigned char base)
  : buffer(0), capacity(0), len(0) {
  concat((long) x, base);
}

String::String(unsigned long x, unsigned char base)
  : buffer(0), capacity(0), len(0) {
  concat((unsigned long) x, base);
}

String::String(float x, unsigned char decimalPlaces)
  : buffer(0), capacity(0), len(0) {
  concat((double) x, decimalPlaces);
}

String::String(double x, unsigned char decimalPlaces)
  : buffer(0), capacity(0), len(0) {
  concat((double) x, decimalPlaces);
}

unsigned char String::reserve(unsigned int _capacity) {
  char *_buffer = (char *) realloc(buffer,_capacity+1);
  if (_buffer == 0) {
    invalidate();
    return false;
  } else {
    buffer = _buffer;
    capacity = _capacity;
    return true;
  }
}

unsigned int String::length() const { return len; }

String& String::operator= (const char *str) {
  len=0;
  concat(str,strlen(str));
  return *this;  
}

String& String::operator= (const String &str) {
  len = 0;
  concat(str.buffer,str.len);
  return *this;  
}

String& String::operator= (String &&str) {
  move(str);
  return *this;  
}

String& String::operator= (StringSumHelper && str) {
  move(str);
  return *this;
}

unsigned char String::concat(const char *x) {
  return concatOk(x,strlen(x));
}
unsigned char String::concat(const __FlashStringHelper *x) {
  return concatOk((const char *) x, strlen((const char *)x));
}
unsigned char String::concat(const String &x) {
  return concatOk(x.buffer,x.len);
}

unsigned char String::concat(char x) {
  if (concatOk((const char *) 0, 1)) {
    buffer[len-1]=x;
    return true;
  } else {
    return false;
  }
}

unsigned char String::concat(unsigned char x,  unsigned char base) {
  return concat((unsigned long) x, base);
}

unsigned char String::concat(int x, unsigned char base) {
  return concat((long) x,base); 
}
unsigned char String::concat(unsigned int x, unsigned char base) {
  return concat((unsigned long) x, base);
}
unsigned char String::concat(float x, unsigned char decimalPlaces) {
  return concat((double) x, decimalPlaces);
}

String& String::operator += (const char *x) {
  concat(x);
  return *this;
}
String& String::operator += (const __FlashStringHelper *x) {
  concat(x);
  return *this;
};  
String& String::operator += (const String &x) {
  concat(x);
  return *this;
}

String& String::operator += (char x) {
  concat(x);
  return *this;
}

String& String::operator += (unsigned char x) {
  concat(x);
  return *this;
}

String& String::operator += (int x) {
  concat(x);
  return *this;
}

String& String::operator += (unsigned int x) {
  concat(x);
  return *this;
}

String& String::operator += (long x) {
  concat(x);
  return *this;  
}

String& String::operator += (unsigned long x) {
  concat(x);
  return *this;
}

String& String::operator += (float x) {
  concat(x);
  return *this;
}

String& String::operator += (double x) {
  concat(x);
  return *this;
}

StringSumHelper& operator + (const StringSumHelper &sum, const char *x) {
  StringSumHelper &a = const_cast < StringSumHelper& >(sum);
  a += x;
  return a;
}
StringSumHelper& operator + (const StringSumHelper &sum, const __FlashStringHelper *x) {
  StringSumHelper &a = const_cast < StringSumHelper& >(sum);
  a += x;
  return a;
}
StringSumHelper& operator + (const StringSumHelper &sum, const String &x) {
  StringSumHelper &a = const_cast < StringSumHelper& >(sum);
  a += x;
  return a;
}
StringSumHelper& operator + (const StringSumHelper &sum, char x) {
  StringSumHelper &a = const_cast < StringSumHelper& >(sum);
  a += x;
  return a;
}
StringSumHelper& operator + (const StringSumHelper &sum, unsigned char x) { 
  StringSumHelper &a = const_cast < StringSumHelper& >(sum);
  a += x;
  return a;
}
StringSumHelper& operator + (const StringSumHelper &sum, int x) {
  StringSumHelper &a = const_cast < StringSumHelper& >(sum);
  a += x;
  return a;
}
StringSumHelper& operator + (const StringSumHelper &sum, unsigned int x) {
  StringSumHelper &a = const_cast < StringSumHelper& >(sum);
  a += x;
  return a;
}
StringSumHelper& operator + (const StringSumHelper &sum, long x) {
  StringSumHelper &a = const_cast < StringSumHelper& >(sum);
  a += x;
  return a;
}
StringSumHelper& operator + (const StringSumHelper &sum, unsigned long x) {
  StringSumHelper &a = const_cast < StringSumHelper& >(sum);
  a += x;
  return a;
}
StringSumHelper& operator + (const StringSumHelper &sum, float x) {
  StringSumHelper &a = const_cast < StringSumHelper& >(sum);
  a += x;
  return a;
}
StringSumHelper& operator + (const StringSumHelper &sum, double x) {
  StringSumHelper &a = const_cast < StringSumHelper& >(sum);
  a += x;
  return a;
}

int String::compareTo(const String &to) const {
  return strcmp(buffer, to.buffer);
}

unsigned char String::equals(const String &to) const { return compareTo(to) == 0; }
unsigned char String::equals(const char *to) const { return strcmp(buffer,to) ==0; }
unsigned char String::operator == (const String &to) const { return strcmp(buffer,to.buffer) == 0; }
unsigned char String::operator == (const  char *to) const { return strcmp(buffer,to) == 0; }
unsigned char String::operator != (const String &to) const { return strcmp(buffer,to.buffer) != 0; }
unsigned char String::operator != (const char *to) const { return strcmp(buffer,to) != 0; }
unsigned char String::operator > (const String &to) const { return strcmp(buffer,to.buffer) > 0; }
unsigned char String::operator < (const String &to) const { return strcmp(buffer,to.buffer) < 0; }
unsigned char String::operator >= (const String &to) const { return strcmp(buffer,to.buffer) >=0; }
unsigned char String::operator <= (const String &to) const { return strcmp(buffer,to.buffer) <= 0; } 
  
unsigned char String::equalsIgnoreCase(const String &to) const {
  return strcasecmp(buffer,to.buffer) == 0;
}
unsigned char String::startsWith(const String &str) const {
  return strncmp(buffer,str.buffer,str.len) == 0;
}
unsigned char String::startsWith(const String &str, unsigned int offset) const {
  if (offset > len) return false;
  return strncmp(buffer+offset,str.buffer,str.len) == 0;
}
unsigned char String::endsWith(const String &str) const {
  if (str.len > len) return false;
  return strncmp(buffer+len-str.len,str.buffer,str.len) == 0;
}

char String::charAt(unsigned int i) const { return i < len ? buffer[i] : 0; }
void String::setCharAt(unsigned int i, char c) { if (i<len) buffer[i]=c; }
char String::operator[] (unsigned int i) const { return i < len ? buffer[i] : 0; }
char& String::operator[] (unsigned int i) { static char err; return (i<len) ? buffer[i] : err; }
void String::getBytes(unsigned char *buf, unsigned int size, unsigned int offset) const {
  if (buf == 0 || size == 0) return;

  int n = len-offset;
  if (n > size-1) n = size-1;
  memmove(buf,buffer+offset,n);
  buf[n]=0;
}

void String::toCharArray(char *buf, unsigned int size, unsigned int offset) const {
  getBytes((unsigned char*)buf, size, offset);
}

const char *String::c_str() const { return buffer; }

char* String::begin() { return buffer; }
char* String::end() { return buffer+len; }
const char* String::begin() const { return buffer; }
const char* String::end() const { return buffer + len; }

int String::indexOf(char c) const { return indexOf(c,0); }

int String::indexOf(char c, unsigned int offset) const {
  if (offset > len) return -1;
  const char *p = strchr(buffer+offset,c);
  return (p != 0 && p-buffer <=len) ? p-buffer : -1;
}

int String::indexOf(const String &str) const {
  return indexOf(str,0);
}
int String::indexOf(const String &str, unsigned int offset) const {
  if (str.len == 0) return offset <= len ? offset : -1;
  if (str.len == 1) return indexOf(str.buffer[0],offset);
  for (;;) {
    int at = indexOf(str.buffer[0],offset);
    if (at == -1 || at > len-str.len) return -1;
    if (strncmp(buffer+at+1,str.buffer+1,str.len-1) == 0) return at;
    offset = at + 1;
  }
}

int String::lastIndexOf(char c) const {
  return (len > 0) ? lastIndexOf(c,len-1) : -1;
}

int String::lastIndexOf(char c, unsigned int offset) const {
  if (len == 0) return -1;
  if (offset >= len) offset = len-1;
  for (;;) {
    if (buffer[offset] == c) return offset;
    if (offset == 0) return -1;
    --offset;
  }
}

int String::lastIndexOf(const String &str) const {
  return lastIndexOf(str,len);
}

int String::lastIndexOf(const String &str, unsigned int offset) const {
  if (offset > len) offset = len;
  if (str.len == 0) return offset;
  if (str.len == 1) return lastIndexOf(str.buffer[0],offset);
  for (;;) {
    int at = lastIndexOf(str.buffer[str.len-1],offset);
    if (at == -1 || at < str.len-1) return -1;
    if (strncmp(buffer+at-(str.len-1),str.buffer,str.len-1)==0) return at - (str.len-1);
    offset = at-1;
  }
}

String String::substring(unsigned int i) const {
  return substring(i,len);
}

String String::substring(unsigned int begin, unsigned int end) const {
  if (begin > len) begin=len;
  if (end > len) end = len;
  return String(buffer+begin,end-begin);
}

void String::replace(char find, char replace) {
  for (int i=0; i<len; ++i) {
    if (buffer[i] == find) buffer[i]=replace;
  }
}

void String::replace(const String &find, const String &replace) {
  unsigned int offset = 0;
  for (;;) {
    int at = indexOf(find,offset);
    if (at == -1) break;
    remove(at,at+find.len);
    insert(at,replace);
    offset = at + replace.len;
  }
}

void String::insert(unsigned at, const String &str) {
  if (str.len > 0) {
    concat((const char *) 0,str.len);
    memmove(buffer+at+str.len,buffer+at,len-str.len);
    memmove(buffer+at,str.buffer,str.len);
  }
}

void String::remove(unsigned int i) {
  remove(i,len);
}

void String::remove(unsigned int begin, unsigned int end) {
  if (begin > len) begin=len;
  if (end > len) end = len;
  if (end <= begin) return;
  memmove(buffer+begin,buffer+end,len-end);
  len = len-(end-begin);
  buffer[len]=0;
}

void String::toLowerCase() {
  for (int i=0; i<len; ++i) {
    buffer[i]=tolower(buffer[i]);
  }
}

void String::toUpperCase() {
  for (int i=0; i<len; ++i) {
    buffer[i]=toupper(buffer[i]);
  }
}

void String::trim() {
  int i = 0;
  while (i < len && isspace(buffer[i])) ++i;
  remove(0,i);
  i = len-1;
  while (i > 0 && isspace(buffer[i])) --i;
  remove(i+1,len);
}

long String::toInt() const { return atol(buffer); }
float String::toFloat() const { return atof(buffer); }
double String::toDouble() const { return atof(buffer); }

void String::init() {
  buffer = 0;
  len = 0;
  capacity = 0;
}

void String::invalidate() {
  if (buffer != 0) free(buffer);
  buffer = 0;
  len = 0;
  capacity = 0;
}

unsigned char String::changeBuffer(unsigned int _capacity) {
  char *_buffer = (char*) realloc(buffer,_capacity);
  if (_buffer != 0) {
    capacity = _capacity;
    buffer = _buffer;
    return true;
  } else {
    return false;
  }
}

String& String::copy(const char *str, unsigned int length) {
  len=0;
  concat(str,length);
  return *this;
}

String& String::copy(const __FlashStringHelper *str, unsigned int length) {
  len=0;
  concat((const char *) str,length);
  return *this;
}

void String::move(String &rhs) {
  if (this == &rhs) return;

  free(buffer);
  buffer=rhs.buffer;
  len=rhs.len;
  capacity=rhs.capacity;
  rhs.buffer=0;
  rhs.capacity=0;
  rhs.len=0;
}

String::~String() {
  if (buffer != 0) free(buffer);
}

std::ostream& operator<<(std::ostream& out, const String &s) {
  out << s.c_str();
  return out;
}

#endif

#include "ArduinoMock2WString.cpp"

#if ! defined(ARDUINO)

unsigned char String::concat(const char *str, unsigned int _len) {
  if (capacity < len+_len) {
    unsigned int _capacity = (len+_len)*5/4+4;
    char *_buffer = (char*) realloc(buffer, _capacity+1);
    if (_buffer == 0) return false;

    // move str ref if it was moved in the realloc...
    if (buffer != 0 && str != 0 && buffer <= str && str <= buffer+capacity) {
      str = str + (_buffer - buffer);
    }
    capacity=_capacity;
    buffer=_buffer;
  }
  if (str != 0) {
    memcpy(buffer+len,str,_len);
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
  if (x < 0) {++sz; x = -x; }
  do { ++sz; x=x/base; } while (x != 0);
  unsigned at = len;
  if (concat((const char *) 0, sz)) {
    x=value;
    if (x < 0) { buffer[at++]='-'; x= -x; }
    do {
      int d = x % base;
      char c = (d < 10) ? d+'0' : d+'a'-10;
      buffer[at++]=c;
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
  if (concat((const char *) 0, sz)) {
    x=value;
    do {
      int d = x % base;
      char c = (d < 10) ? d+'0' : d+'a'-10;
      buffer[at++]=c;
      x = x/base;
    } while (x != 0);
    return true;
  } else {
    return false;
  }
}

unsigned String::concat(double value, unsigned char precision) {
  double x = value;
  unsigned char p = precision;
  if (x < 0) { x = -x; }
  double q=1; 
  while (p > 0) { q=q*10; --p; }
  x = floor(x*q+0.5);
  sz=(value < 0) + (precision+1);
  while (x >= q) { q=q*10; ++sz; }
  unsigned at = len;
  x = x/q;
  if (concat((const char *) 0, sz)) {
    if (value < 0) {
      buffer[at++]='-';
      --sz;
    }
    while (sz > 0) {
      x=10*x;
      c=floor(x);
      x=x-c;
      buffer[at++]=c+'0';
      --sz;
      if (sz == precision + 1) {
        buffer[at++]='.';
        --sz;
      }
    }
    return true;
  } else {
    return false;
  }
}

// String(unsigned int _capacity, const char *fmt,...) {
//   va_list args;
//   va_start(args, fmt);
//   capacity = _capacity;
//   buffer = (char*) malloc(capacity+1);
//   vsnprintf(buffer,capacity+1,format,args);
// }

String::String(const char *str)
  : buffer(0), capacity(0), len(0) {
  concat(str,strlen(str));
}

String::String(const String &str)
  : buffer(0), capacity(0), len(0) {
  concat(str.buffer,str.len);
}

String::String(const __FlashStringHelper *str)
  : buffer(0), capacity(0), len(0) {
  concat((const char *) str, strlen((const char *) str));
}

String::String(char x)
  : buffer(0), capacity(0), len(0) {
  concat((const char *)0,1);
  buffer[0] = x;
}

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
  char *_buffer = realloc(_capacity+1, buffer);
  if (_buffer != 0) {
    buffer=_buffer;
    capacity=_capacity;
    return true;
  } else {
    return false;
  }
}

unsigned int String::length() const { return len; }

String& operator= (const char *str) {
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
  return concat(x,strlen(x));
}
unsigned char String::concat(const __FlashStringHelper *x) {
  return concat((const char *) x);
}
unsigned char String::concat(const String &x) {
  return concat(x.buffer,x.len);
}

unsigned char String::concat(char x) {
  if (concat((const char *) 0, 1)) {
    buffer[len-1]=x;
    return true;
  } else {
    return false;
  }
}
unsigned char String::concat(unsigned char x) {
  return concat((unsigned long) x);
}
unsigned char String::concat(int x) {
  return concat((long) x); 
}
unsigned char String::concat(unsigned int x) {
  return concat((unsigned long) x)
}
unsigned char String::concat(float x) {
  return concat((double) x);
}

String& operator += (const char *x) {
  concat(x);
  return *this;
}
String& operator += (const __FlashStringHelper *x) {
  concat(x);
  return *this;
};  
String& operator += (const String &x) {
  concat(x);
  return *this;
}

String& operator += (char x) {
  concat(x);
  return *this;
}

String& operator += (unsigned char x) {
  concat(x);
  return *this;
}

String& operator += (int x) {
  concat(x);
  return *this;
}

String& operator += (unsigned int x) {
  concat(x);
  return *this;
}

String& operator += (long x) {
  concat(x);
  return *this;  
}

String& operator += (unsigned long x) {
  concat(x);
  return *this;
}

String& operator += (float c) {
  concat(x);
  return *this;
}

String& operator += (double c) {
  concat(x);
  return *this;
}


StringSumHelper& operator + (const StringSumHelper &sum, const char *x) { concat(x); }
StringSumHelper& operator + (const StringSumHelper &sum, const __FlashStringHelper *x) { concat(x); }  
StringSumHelper& operator + (const StringSumHelper &sum, const String &x) { concat(x); }
StringSumHelper& operator + (const StringSumHelper &sum, char x) { concat(x); }
StringSumHelper& operator + (const StringSumHelper &sum, unsigned char x) { concat(x); }
StringSumHelper& operator + (const StringSumHelper &sum, int x) { concat(x); }
StringSumHelper& operator + (const StringSumHelper &sum, unsigned int x) { concat(x); }
StringSumHelper& operator + (const StringSumHelper &sum, long x) { concat(x); }
StringSumHelper& operator + (const StringSumHelper &sum, unsigned long x) { concat(x); }
StringSumHelper& operator + (const StringSumHelper &sum, float x) { concat(x); }
StringSumHelper& operator + (const StringSumHelper &sum, double x) { concat(x); }

int String::compareTo(const String &to) const {
  return strcmp(buffer, to.buffer);
}

unsigned char String::equals(const String &to) const { return compareTo(to) == 0; }
unsigned char String::equals(const char *to) const { strcmp(buffer,to) ==0; }
unsigned char String::operator == (const String &to) const { return strcmp(buffer,to.buffer) == 0; }
unsigned char String::operator == (const  char *to) const { return strcmp(buffer,to) == 0; }
unsigned char String::operator != (const String &to) const { return strcmp(buffer,to.buffer) != 0; }
unsigned char String::operator != (const char *to) const { return strcmp(buffer,to) != 0; }
unsigned char String::operator > (const String &to) const { return strcmp(buffer,to) > 0; }
unsigned char String::operator < (const String &to) const { return strcmp(buffer,to) < 0; }
unsigned char String::operator >= (const String &to) const { return strcmp(buffer,to) >=0; }
unsigned char String::operator <= (const String &to) const { return strcmp(buffer,to) <= 0; } 
  
unsigned char equalsIgnoreCase(const String &str) const;
unsigned char startsWith(const String &str) const;
unsigned char startsWith(const String &str, unsigned int offset) const;
unsigned char endsWith(const String &str) const;

char String::charAt(unsigned int i) const { return i < len ? buffer[i] : 0; }
void String::setCharAt(unsigned int i, char c) { if (i<len) buffer[i]=c; }
char String::operator[] (unsigned int i) const { return i < len ? buffer[i] : 0; }
char& String::operator[] (unsigned int i) { static char err; return (i<len) ? buffer[i] : err; }
void String::getBytes(unsigned char *buf, unsigned int size, unsigned int offset) const {
  if (buf == 0 || size == 0) return;

  int n = len-offset;
  if (n > size-1) n = size-1;
  memcpy(buf,buffer+offset,n);
  buf[n]=0;
}

void String::toCharArray(char *buf, unsigned int size, unsigned int offset) const {
  getBytes((unsigned char*)buf, size, offset);
}

public: const char *String::c_str() const { return buffer; }

char* String::begin() { return buffer; }
char* String::end() { return buffer+len; }
const char* String::begin() const { return buffer; }
const char* String::end() const { return buffer + end; }

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
  return lastIndexOf(c,len);
}

int String::lastIndexOf(char c, unsigned int offset) const {
  if (offset > len) offset = len;
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
    int at = lastIndexOf(str.buffer[len-1],offset);
    if (at == -1 || at < str.len) return -1;
    if (strncmp(buffer+at-str.len,str.buffer,str.len-1)==0) return at - (str.len-1);
    offset = at-1;
  }
}

String String::substring(unsigned int i) {
  return substring(i,len);
}

String String::substring(unsigned int begin, unsigned int end) {
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

void String::insert(unsigned at, const String &replace) {
  if (replace.len > 0) {
    concat((const char *) 0,replace.len);
    memcpy(buffer+at+replace.len,buffer+at,buffer.len-replace.len);
    memcpy(buffer+at,replace.buffer,replace.len);
  }
}

void String::remove(unsigned int i) {
  remove(i,len);
}

void String::remove(unsigned int begin, unsigned int end) {
  if (begin > len) begin=len;
  if (end > len) end = len;
  if (end <= begin) return;
  memcpy(buffer+begin,buffer+end,len-end);
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
  remove(i,len);
}

long String::toInt() const { return atol(buffer); }
float String::toFloat() const { return atof(buffer); }
double String::toDouble() const { return atof(buffer); }

void String::init() { /* what */ }
void String::invalidate() { /* what */ }
unsigned char String::changeBuffer(unsigned int _capacity) {
  char *_buffer = realloc(buffer,_capacity);
  if (_buffer != 0) {
    capacity = _capacity;
    buffer = _buffer;
    return true;
  } else {
    return false;
  }
}

String& copy(const char *str, unsigned int length) {
  len=0;
  concat(str,length);
}

String& copy(const char __FlashStringHelper *str, unsigned int length) {
  len=0;
  concat((const char *) str,length);
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

#endif

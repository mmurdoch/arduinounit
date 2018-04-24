#pragma once

#if defined(ARDUINO)
#include "WString.h"
#else
#include <iostream>
#include "Flash.h"

class __FlashStringHelper;

class StringSumHelper;

class String
{
 private: String(const char * str, unsigned int len);
 public: String(const char * str="");
 public: String(const String &str);
 public: String(const __FlashStringHelper *str);
 public: explicit String(char x);
 public: explicit String(unsigned char, unsigned char base=10);
 public: explicit String(int x, unsigned char base=10);
 public: explicit String(unsigned int x, unsigned char base=10);
 public: explicit String(long x, unsigned char base=10);
 public: explicit String(unsigned long x, unsigned char base=10);
 public: explicit String(float x, unsigned char decimalPlaces=2);
 public: explicit String(double x, unsigned char decimalPlaces=2);
 public: unsigned char reserve(unsigned int size);
 public: unsigned int length() const;
 public: String& operator= (const char *str);
 public: String& operator= (const String &str);
 public: String& operator= (String &&str);
 public: String& operator= (StringSumHelper && str);
 public: unsigned char concat(const char *x);
 public: unsigned char concat(const __FlashStringHelper *x);  
 public: unsigned char concat(const String &x);
 public: unsigned char concat(char x);    
 public: unsigned char concat(unsigned char x) { return concat(x,10); }
 private: unsigned char concat(unsigned char x, unsigned char base);    
 public: unsigned char concat(int x) { return concat(x,10); };
 private: unsigned char concat(int x, unsigned char base);    
 public: unsigned char concat(unsigned int x) { return concat(x,10); };
 private: unsigned char concat(unsigned int x, unsigned char base);      
 public: unsigned char concat(long x) { return concat(x,10); }
 private: unsigned char concat(long x, unsigned char base);        
 public: unsigned char concat(unsigned long x) { return concat(x,10); }
 private: unsigned char concat(unsigned long x, unsigned char base);          
 public: unsigned char concat(float x) { return concat(x,2); }
 private: unsigned char concat(float x, unsigned char decimalPlaces);
 public: unsigned char concat(double x) { return concat(x,2); }
 private: unsigned char concat(double x, unsigned char decimalPlaces);  
 public: String& operator += (const char *str);
 public: String& operator += (const __FlashStringHelper *str);  
 public: String& operator += (const String &str);
 public: String& operator += (char c);    
 public: String& operator += (unsigned char c);  
 public: String& operator += (int c);    
 public: String& operator += (unsigned int c);  
 public: String& operator += (long c);    
 public: String& operator += (unsigned long c);  
 public: String& operator += (float c);    
 public: String& operator += (double c);
  
 public: friend StringSumHelper& operator + (const StringSumHelper &sum, const char *str);
 public: friend StringSumHelper& operator + (const StringSumHelper &sum, const __FlashStringHelper *str);  
 public: friend StringSumHelper& operator + (const StringSumHelper &sum, const String &str);
 public: friend StringSumHelper& operator + (const StringSumHelper &sum, char c);    
 public: friend StringSumHelper& operator + (const StringSumHelper &sum, unsigned char c);  
 public: friend StringSumHelper& operator + (const StringSumHelper &sum, int c);    
 public: friend StringSumHelper& operator + (const StringSumHelper &sum, unsigned int c);  
 public: friend StringSumHelper& operator + (const StringSumHelper &sum, long c);    
 public: friend StringSumHelper& operator + (const StringSumHelper &sum, unsigned long c);  
 public: friend StringSumHelper& operator + (const StringSumHelper &sum, float c);    
 public: friend StringSumHelper& operator + (const StringSumHelper &sum, double c);

 public: int compareTo(const String &to) const;
 public: unsigned char equals(const String &to) const;
 public: unsigned char equals(const char *to) const;
 public: unsigned char operator == (const String &to) const;
 public: unsigned char operator == (const  char *to) const;
 public: unsigned char operator != (const String &to) const;
 public: unsigned char operator != (const char *to) const;
 public: unsigned char operator > (const String &to) const;
 public: unsigned char operator < (const String &to) const;
 public: unsigned char operator >= (const String &to) const;
 public: unsigned char operator <= (const String &to) const;
  
 public: unsigned char equalsIgnoreCase(const String &str) const;
 public: unsigned char startsWith(const String &str) const;
 public: unsigned char startsWith(const String &str, unsigned int offset) const;
 public: unsigned char endsWith(const String &str) const;

 public: char charAt(unsigned int i) const;
 public: void setCharAt(unsigned int i, char c);
 public: char operator[] (unsigned int i) const;
 public: char& operator[] (unsigned int i);
 public: void getBytes(unsigned char *buf, unsigned int size, unsigned int offset=0) const;
 public: void toCharArray(char *buf, unsigned int size, unsigned int offset=0) const;
 public: const char *c_str() const;
 public: char* begin();
 public: char* end();
 public: const char* begin() const;
 public: const char* end() const;
 public: int indexOf(char c) const;
 public: int indexOf(char c, unsigned int offset) const;
 public: int indexOf(const String &str) const;  
 public: int indexOf(const String &str, unsigned int offset) const;
 public: int lastIndexOf(char c) const;
 public: int lastIndexOf(char c, unsigned int offset) const;
 public: int lastIndexOf(const String &str) const;  
 public: int lastIndexOf(const String &str, unsigned int offset) const;
 public: String substring(unsigned int i) const;
 public: String substring(unsigned int begin, unsigned int end) const;
 public: void replace(char find, char replace);
 public: void replace(const String &find, const String &replace);
 public: void remove(unsigned int i);
 public: void remove(unsigned int begin, unsigned int end);
 private: void insert(unsigned int at, const String &str);
 public: void toLowerCase();
 public: void toUpperCase();
 public: void trim();

 public: long toInt() const;
 public: float toFloat() const;
 public: double toDouble() const;

 protected: char *buffer;
 protected: unsigned int capacity;
 protected: unsigned int len;
 protected: void init();
 protected: void invalidate();
 protected: unsigned char changeBuffer(unsigned int maxStrLen);
 private: unsigned char concatOk(const char *str, unsigned int length);  
 protected: void concat(const char *str, unsigned int length);
 protected: String& copy(const char *str, unsigned int length);
 protected: String& copy(const __FlashStringHelper *str, unsigned int length);
 protected: void move(String &rhs);
 public: ~String();
};

struct StringSumHelper : String {
 public: StringSumHelper(const String &x) : String(x) {}
 public: StringSumHelper(const char *x) : String(x) {}  
 public: StringSumHelper(char x) : String(x) {}
 public: StringSumHelper(unsigned char x) : String(x) {}
 public: StringSumHelper(int x) : String(x) {}
 public: StringSumHelper(unsigned int x) : String(x) {}
 public: StringSumHelper(long x) : String(x) {}
 public: StringSumHelper(unsigned long x) : String(x) {}
 public: StringSumHelper(float x) : String(x) {}
 public: StringSumHelper(double x) : String(x) {}
};

std::ostream& operator<<(std::ostream & out, const String &s);

#endif

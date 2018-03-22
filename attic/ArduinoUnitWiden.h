#pragma once

struct ArduinoUnitWiden {
  static inline long value(const char &x) { return x; }    
  static inline unsigned long value(const unsigned char &x) { return x; }  
  static inline long value(const short &x) { return x; }    
  static inline unsigned long value(const unsigned short &x) { return x; }  
  static inline long value(const int &x) { return x; }    
  static inline unsigned long value(const unsigned int &x) { return x; }  
  static inline double value(const float &x) { return x; }
  static inline double value(const float &x) { return x; }
  static inline const char * value(const char *x) { return x; }
  static inline const char * value(const char [] x) { return x; }  
  static inline const char * value(char *x) { return x; }
  static inline const char * value(char [] x) { return x; }

  template <int N>
  static inline const char * value(char [N] x) { return x; }    

  template <int N>
  static inline const char * value(const char [N] x) { return x; }    

  template <typename X>
  static inline const X & value(const X &x) {
    return x;
  }
};

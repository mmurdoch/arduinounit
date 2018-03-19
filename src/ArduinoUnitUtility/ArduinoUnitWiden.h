#pragma once

struct ArduinoUnitWiden {
  static inline long value(const char &x) { return x; }    
  static inline unsigned long value(const unsigned char &x) { return x; }  
  static inline long value(const short &x) { return x; }    
  static inline unsigned long value(const unsigned short &x) { return x; }  
  static inline long value(const int &x) { return x; }    
  static inline unsigned long value(const unsigned int &x) { return x; }  
  static inline double value(const float &x) { return x; }

  template <typename X>
  static inline const X & value(const X &x) {
    return x;
  }
};



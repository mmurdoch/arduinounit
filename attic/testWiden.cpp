#include <iostream>
#include <typeinfo>
#include <stdint.h>


struct Log {
  bool i_am_a_log;
  Log(bool _i_am_a_log) : i_am_a_log(_i_am_a_log) {}
};

int main() {
  Log y(true);
  int i = 3;
  uint8_t b = 4;
  int32_t x = 4;
  int64_t z = 4;  
  float f=1;
  double d=2;

  std::cout << typeid(Widen::widen(b)).name() << std::endl;    
  std::cout << typeid(Widen::widen(i)).name() << std::endl;
  std::cout << typeid(Widen::widen(x)).name() << std::endl;    
  std::cout << typeid(Widen::widen(y)).name() << std::endl;
  std::cout << typeid(Widen::widen(z)).name() << std::endl;  
  std::cout << typeid(Widen::widen(f)).name() << std::endl;
  std::cout << typeid(Widen::widen(d)).name() << std::endl;  

  return 0;
}

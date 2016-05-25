#include <string.h>
#include <iostream>

#include "Compare.h"

#if 0
template <typename A, typename B>
struct Compare
{
  inline static int cmp(const A &a, const B &b)
  {
    if (a<b) return -1;
    if (b<a) return  1;
    return 0;
  }
};

template <>
struct Compare<const char *, const char *>
{
  inline static int cmp(const char* const &a, const char* const &b)
  {
    std::cout << "specialized on line " << (__LINE__+1) << std::endl;    
    return strcmp(a,b);
  }
};

template <>
struct Compare<const char *, char *>
{
  inline static int cmp(const char* const &a, char* const &b)
  {
    std::cout << "specialized on line " << (__LINE__+1) << std::endl;    
    return strcmp(a,b);
  }
};

template <>
struct Compare<char *, const char *>
{
  inline static int cmp(char* const &a, const char* const &b)
  {
    std::cout << "specialized on line " << (__LINE__+1) << std::endl;    
    return strcmp(a,b);
  }
};

template <>
struct Compare<char *, char *>
{
  inline static int cmp(char* const &a, char* const &b)
  {
    std::cout << "specialized on line " << (__LINE__+1) << std::endl;    
    return strcmp(a,b);
  }
};

template <std::size_t N>
struct Compare<char [N], const char *>
{
  inline static int cmp(const char (&a)[N], const char* const &b)
  {
    std::cout << "specialized on line " << (__LINE__+1) << std::endl;    
    return strcmp(a,b);
  }
};

template <std::size_t N>
struct Compare<const char *, char [N]>
{
  inline static int cmp(const char* const &a, const char (&b)[N])
  {
    std::cout << "specialized on line " << (__LINE__+1) << std::endl;    
    return strcmp(a,b);
  }
};

template <std::size_t N, std::size_t M>
struct Compare<char [N], char [M]>
{
  inline static int cmp(const char (&a)[N], const char (&b)[M])
  {
    std::cout << "specialized on line " << (__LINE__+1) << std::endl;    
    return strcmp(a,b);
  }
};

template <std::size_t N>
struct Compare<char [N], char *>
{
  inline static int cmp(const char (&a)[N], char* const &b)
  {
    std::cout << "specialized on line " << (__LINE__+1) << std::endl;    
    return strcmp(a,b);
  }
};

template <std::size_t N>
struct Compare<char *, char [N]>
{
  inline static int cmp(char* const &a, const char (&b)[N])
  {
    std::cout << "specialized on line " << (__LINE__+1) << std::endl;    
    return strcmp(a,b);
  }
};
#endif

template <typename A, typename B>
inline int compare(const A &a, const B &b)
{
  return Compare<A,B>::cmp(a,b);
}

#define check(a,b) { \
    int cmp = compare(a,b); \
    std::cout << __LINE__ << ": compare(" << (a) << "," << (b) << ")=" << cmp <<  std::endl; \
  }

const char *a="42";
const char b[3]={'4','2',0};
char *c="42";
char d[3]={'4','2',0};

int main(int argc, char *argv[]) 
{

  std::cout << "strcmp(a,b)=" << strcmp("a","b") << std::endl;
  
  check(a,a);
  check(a,b);
  check(a,c);
  check(a,d);

  check(b,a);  
  check(b,b);
  check(b,c);
  check(b,d);

  check(c,a);  
  check(c,b);  
  check(c,c);
  check(c,d);

  check(d,a);  
  check(d,b);  
  check(d,c);
  check(d,d);

  check(2,3);
  check(3,3);
  check(3,2);

  check("a","b");
  check("b","b");
  check("b","a");

  return 0;
}

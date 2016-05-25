#include <iostream>
#include <fstream>

struct MkCompare
{
  std::ostream &out;

  typedef enum { GENERIC=0, CONST_CHAR_PTR, CHAR_PTR, FLASH_CHAR_PTR, SIZED_ARRAY, GENERIC_ARRAY, STRING } TYPES;

  static const TYPES SPECIAL[];

  int phase;
  TYPES typeA;
  TYPES typeB;

  bool useStrcmp() { return (typeA != GENERIC && typeB != GENERIC); }
  bool useLess() { return true; }

  void methodArg(TYPES T, char name) {
    switch(T) {
    case GENERIC: out << "const " << (name == 'a' ? 'A' : 'B') << " &" << name; break;
    case CONST_CHAR_PTR: out << "const char * const &" << name; break;
    case FLASH_CHAR_PTR: out << "const __FlashStringHelper * const &" << name; break;
    case CHAR_PTR: out << "char * const &" << name; break;
    case SIZED_ARRAY: out << "const char (&" << name << ")[" << (name == 'a' ? "N" : "M") <<"]"; break;
    case GENERIC_ARRAY: out << "const char (&" << name << ")[]"; break;
    case STRING: out << "const String &" << name; break;      
    default: break;
    };
  }

  bool templateDeclareArg(TYPES T, char name, bool comma=false) {
    switch(T) {
    case GENERIC: out << (comma ? ", " : "") << "typename " << (name == 'a' ? "A" : "B"); return true;
    case SIZED_ARRAY: out << (comma ? ", " : "") << "size_t " << (name == 'a' ? "N" : "M"); return true;
    default: return false;      
    }
  }

  void templateDeclare() {
    out << " < ";
    bool comma=templateDeclareArg(typeA,'a');
    templateDeclareArg(typeB,'b',comma);
    out << " > ";
  }
  
  void templateArg(TYPES T, char name) {
    switch(T) {
    case CONST_CHAR_PTR: out << "const char *"; break;
    case CHAR_PTR: out << "char *"; break;
    case FLASH_CHAR_PTR: out << "const __FlashStringHelper *"; break;      
    case SIZED_ARRAY: out << "char [" << (name == 'a' ? "N" : "M") <<"]"; break;
    case GENERIC_ARRAY: out << "char []"; break;
    case STRING: out << "String"; break;      
    default: break;
    };
  }

  void templateArgs() {
    if (typeA != GENERIC || typeB != GENERIC) {
      out << "<";
      templateArg(typeA,'a');
      out << ",";
      templateArg(typeB,'b');
      out << ">";
    }
  }

  MkCompare(std::ostream &_out) : out(_out)
  {
    typeA=GENERIC;
    typeB=GENERIC;
    phase=1;
  }

  virtual void print() {
    mask();
    head();
    if (phase > 0) {
      between();
      equal();
      notEqual();
      less();
      more();
      lessOrEqual();
      moreOrEqual();
      foot();
    }
    unmask();
  };

  bool masked() {
    return typeA == FLASH_CHAR_PTR || typeB == FLASH_CHAR_PTR;
  }

  void mask() {
    if (masked()) {
      out << "#if defined(F)" << std::endl;
    }
  }

  void unmask() {
    if (masked()) {
      out << "#endif" << std::endl;
    }
  }

  virtual void args() {
    out << "(";
    methodArg(typeA,'a');
    out << ",";
    methodArg(typeB,'b');
    out << ")";
  }

  virtual void between() {
    out << "  inline static int between"; args(); out << std::endl;
    out << "  {" << std:: endl;
    if (useStrcmp()) {
      if (typeA != FLASH_CHAR_PTR && typeB != FLASH_CHAR_PTR) {
        if (typeA == STRING) {
          out << "    return a.compareTo(b);" << std::endl;
        } else if (typeB == STRING) {
          out << "    return -b.compareTo(a);" << std::endl;
        } else {
          out << "    return strcmp(a,b);" << std::endl;
        }
      } else if ((typeA != FLASH_CHAR_PTR && typeA != STRING) && typeB == FLASH_CHAR_PTR) {
        out << "    return strcmp_P(a,(const char *)b);" << std::endl;
      } else if (typeA == FLASH_CHAR_PTR && (typeB != FLASH_CHAR_PTR && typeB != STRING)) {
        out << "    return -strcmp_P(b,(const char *)a);" << std::endl;
      } else {
        if (typeB == STRING) {
          out << "    return -Compare < "; templateArg(typeB,'b'); out << ","; templateArg(typeA,'a'); out << " >::between(b,a);" << std::endl;
        } else {
          out << "    uint8_t a_buf[4],b_buf[4];" << std::endl;
          out << "    uint16_t i=0;" << std::endl;
          out << std::endl;
          out << "    for (;;) {" << std::endl;
          out << "      uint8_t j=(i%4);" << std::endl;
          out << "      if (j == 0) {" << std::endl;
          if (typeA == STRING) {
            out << "         a.getBytes(a_buf,4,i);" << std::endl;            
          } else {
            out << "         memcpy_P(a_buf,((const char *)a)+i,4);" << std::endl;
          }
          out << "         memcpy_P(b_buf,((const char *)b)+i,4);" << std::endl;        
          out << "      }" << std::endl;
          out << "      if (a_buf[j] < b_buf[j]) return -1;" << std::endl;
          out << "      if (a_buf[j] > b_buf[j]) return  1;" << std::endl;
          out << "      if (a_buf[j] == 0) return 0;" << std::endl;
          out << "      ++i;" << std::endl;
          out << "    }" << std::endl;
        }
      }
    } else {
      out << "    if (a<b) return -1;" << std:: endl;
      out << "    if (b<a) return  1;" << std:: endl;
      out << "    return 0;" << std:: endl;
    }
    out << "  } // between" << std:: endl;
  }

  virtual void equal() {
    out << "  inline static bool equal"; args(); out << std::endl;
    out << "  {" << std:: endl;
    if (useStrcmp()) {
      out << "    return between(a,b) == 0;" << std::endl;
    } else {
      if (useLess()) {
        out << "    return (!(a<b)) && (!(b<a));" << std::endl;
      } else {
        out << "    return a==b;" << std:: endl;
      }
    }
    out << "  } // equal" << std:: endl;
  }

  virtual void notEqual() {
    out << "  inline static bool notEqual"; args(); out << std::endl;
    out << "  {" << std:: endl;
    if (useStrcmp()) {
      out << "    return between(a,b) != 0;" << std::endl;
    } else {
      if (useLess()) {
        out << "    return (a<b) || (b<a);" << std::endl;
      } else {
        out << "    return !(a==b);" << std:: endl;
      }
    }
    out << "  } // notEqual" << std:: endl;
  }

  virtual void less() {
    out << "  inline static bool less"; args(); out << std::endl;
    out << "  {" << std:: endl;
    if (useStrcmp()) {
      out << "    return between(a,b) < 0;" << std::endl;
    } else {
        out << "    return a<b;" << std:: endl;
    }
    out << "  } // less" << std:: endl;
  }

  virtual void more() {
    out << "  inline static bool more"; args(); out << std::endl;
    out << "  {" << std:: endl;
    if (useStrcmp()) {
      out << "    return between(a,b) > 0;" << std::endl;
    } else {
      out << "    return b<a;" << std:: endl;
    }
    out << "  } // more" << std:: endl;
  }

  virtual void lessOrEqual() {
    out << "  inline static bool lessOrEqual"; args(); out << std::endl;
    out << "  {" << std:: endl;
    if (useStrcmp()) {
      out << "    return between(a,b) <= 0;" << std::endl;
    } else {
      out << "    return !(b<a);" << std:: endl;
    }
    out << "  } // lessOrEqual" << std:: endl;
  }

  virtual void moreOrEqual() {
    out << "  inline static bool moreOrEqual"; args(); out << std::endl;
    out << "  {" << std:: endl;
    if (useStrcmp()) {
      out << "    return between(a,b) >= 0;" << std::endl;
    } else {
      out << "    return !(a<b);" << std:: endl;
    }
    out << "  } // moreOrEqual" << std:: endl;
  }
  
  virtual void head() {
    out << "template ";
    templateDeclare();
    out << "struct Compare"; templateArgs();
    if (phase == 0) {
      out << ";" << std::endl;
    } else {
      out << std:: endl;
      out << "{" << std:: endl;
    }
  }

  virtual void foot() {
    out << "};" << std:: endl;
  }
};

void mkTemplate()
{
  
  std::cout << "template <typename A, typename B>" << std:: endl;
  std::cout << "struct Compare" << std:: endl;
  std::cout << "{" << std:: endl;
  std::cout << "  inline static int cmp(const A &a, const B &b)" << std:: endl;
  std::cout << "  {" << std:: endl;
  std::cout << "    if (a<b) return -1;" << std:: endl;
  std::cout << "    if (b<a) return  1;" << std:: endl;
  std::cout << "    return 0;" << std:: endl;
  std::cout << "  }" << std:: endl;
  std::cout << "};" << std:: endl;
}

const MkCompare::TYPES MkCompare::SPECIAL [] = {
  MkCompare::STRING, MkCompare::CONST_CHAR_PTR, MkCompare::FLASH_CHAR_PTR,MkCompare::CHAR_PTR,
  MkCompare::SIZED_ARRAY, /* MkCompare::GENERIC_ARRAY, */ 
};


int main(int argc, const char *argv[])
{
  bool includeFlash = false;
  bool includeString = false;

  for (int argi=1; argi<argc; ++argi) {
    if (strcmp(argv[argi],"--flash")==0) { includeFlash=true; continue; }
    if (strcmp(argv[argi],"--string")==0) { includeString=true; continue; }
    std::cout << "usage: mkCompare [--flash] [--string]" << std::endl;
    exit(1);
  }

  std::ofstream out("Compare.h");

  MkCompare mk(out);  

  mk.out << "#pragma once" << std::endl;

  if (includeFlash) {
    mk.out << "#if defined(F)" << std::endl;
    mk.out << "#include <avr/pgmspace.h>" << std::endl;
    mk.out << "#endif" << std::endl;    
  }

  if (includeString) {
    mk.out << "#include <WString.h>" << std::endl;
  }

  mk.out << std::endl;

  {
    mk.typeA=MkCompare::GENERIC;
    mk.typeB=MkCompare::GENERIC;
    mk.print();
  }

  for (mk.phase = 0; mk.phase < 2; ++mk.phase) {


    for (int i=0; i<sizeof(MkCompare::SPECIAL)/sizeof(MkCompare::TYPES); ++i) {
      for (int j=0; j<sizeof(MkCompare::SPECIAL)/sizeof(MkCompare::TYPES); ++j) {
        mk.typeA=MkCompare::SPECIAL[i];
        mk.typeB=MkCompare::SPECIAL[j];
        
        if ((!includeFlash)
            && (mk.typeA == MkCompare::FLASH_CHAR_PTR || mk.typeB == MkCompare::FLASH_CHAR_PTR)) continue;
        
        if ((!includeString)
            && (mk.typeA == MkCompare::STRING || mk.typeB == MkCompare::STRING)) continue;
        
        mk.print();
      }
    }
  }

  mk.out << "template <typename A, typename B> int compareBetween(const A &a, const B &b) { return Compare<A,B>::between(a,b); }" << std::endl;
  mk.out << "template <typename A, typename B> bool compareEqual(const A &a, const B &b) { return Compare<A,B>::equal(a,b); }" << std::endl;
  mk.out << "template <typename A, typename B> bool compareNotEqual(const A &a, const B &b) { return Compare<A,B>::notEqual(a,b); }" << std::endl;
  mk.out << "template <typename A, typename B> bool compareLess(const A &a, const B &b) { return Compare<A,B>::less(a,b); }" << std::endl;
  mk.out << "template <typename A, typename B> bool compareMore(const A &a, const B &b) { return Compare<A,B>::more(a,b); }" << std::endl;  
  mk.out << "template <typename A, typename B> bool compareLessOrEqual(const A &a, const B &b) { return Compare<A,B>::lessOrEqual(a,b); }" << std::endl;
  mk.out << "template <typename A, typename B> bool compareMoreOrEqual(const A &a, const B &b) { return Compare<A,B>::moreOrEqual(a,b); }" << std::endl;  

  return 0;
}

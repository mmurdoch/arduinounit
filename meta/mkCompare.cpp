#include <iostream>
#include <fstream>

struct MkCompare
{
  std::ostream &out;

  typedef enum { GENERIC=0, CONST_CHAR_PTR, CHAR_PTR, FLASH_CHAR_PTR, SIZED_ARRAY, GENERIC_ARRAY, STRING } TYPES;

  static const TYPES SPECIAL[];

  const char* typeName(TYPES T) {
    switch(T) {
    case GENERIC: return "GENERIC";
    case CONST_CHAR_PTR: return "CONST_CHAR_PTR";
    case FLASH_CHAR_PTR: return "FLASH_CHAR_PTR";
    case CHAR_PTR: return "CHAR_PTR";
    case SIZED_ARRAY: return "SIZED_ARRAY";
    case GENERIC_ARRAY: return "GENERIC_ARRAY";
    case STRING: return "STRING";
    default: return "UNKNOWN";
    };
  }

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

  void testString(TYPES T, char name) {
    switch(T) {
    case GENERIC: out << "ArduinoUnitString(String( " << name << "))"; break;
    default: out << "ArduinoUnitString(" << name << ")"; break;
    }

    /*    switch(T) {
    case GENERIC: out << "ArduinoUnitString(String( " << name << "))"; break;
    case CONST_CHAR_PTR: out << "ArduinoUnitString(" << name << ")"; break;
    case FLASH_CHAR_PTR: out << "ArduinoUnitString(" << name << ")"; break;
    case CHAR_PTR: out << "ArduinoUnitString((const char *) " << name << ")"; break;
    case SIZED_ARRAY: out << "ArduinoUnitString((const char *) " << name << ")"; break;
    case GENERIC_ARRAY: out << "ArduinoUnitString((const char *) " << name << ")"; break;
    case STRING: out << "ArduinoUnitString(" << name << ".c_str())"; break;
    default: break;
    }; */
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

  bool maskedFlash() {
    return typeA == FLASH_CHAR_PTR || typeB == FLASH_CHAR_PTR;
  }

  bool maskedString() {
    return typeA == STRING || typeB == STRING;
  }

  bool masked() {
    return maskedFlash() || maskedString();
  }

  void maskFlash() {
    if (maskedFlash()) {
      out << "#if ARDUINO_UNIT_USE_FLASH  > 0" << std::endl;
    }
  }

  void maskString() {
    if (maskedString()) {
      out << "#if defined(ARDUINO)" << std::endl;
    }
  }
  
  void mask() {
    maskFlash();
    maskString();
  }
  
  void unmaskFlash() {
    if (maskedFlash()) {
      out << "#endif" << std::endl;
    }
  }

  void unmaskString() {
    if (maskedString()) {
      out << "#endif" << std::endl;
    }
  }
  
  void unmask() {
    unmaskString();
    unmaskFlash();
  }

  virtual void args() {
    out << "(";
    methodArg(typeA,'a');
    out << ",";
    methodArg(typeB,'b');
    out << ")";
  }

  virtual void between() {
    out << "  inline static int8_t between"; args(); out << std::endl;
    out << "  {" << std:: endl;
    if (useStrcmp()) {
      if (typeA != FLASH_CHAR_PTR && typeB != FLASH_CHAR_PTR) {
        if (typeA == STRING) {
          out << "    int ans = a.compareTo(b);" << std::endl;
          out << "    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;" << std::endl;
        } else if (typeB == STRING) {
          out << "    int ans = -b.compareTo(a);" << std::endl;
          out << "    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;" << std::endl;
        } else {
          out << "    int ans = strcmp(a,b);" << std::endl;
          out << "    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;" << std::endl;
        }
      } else if ((typeA != FLASH_CHAR_PTR && typeA != STRING) && typeB == FLASH_CHAR_PTR) {
          out << "    int ans = strcmp_P(a,(const char *)b);" << std::endl;
          out << "    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;" << std::endl;
      } else if (typeA == FLASH_CHAR_PTR && (typeB != FLASH_CHAR_PTR && typeB != STRING)) {
        out << "    int ans = -strcmp_P(b,(const char *)a);" << std::endl;
        out << "    return ans == 0 ? 0 : (ans > 0) ? 1 : -1;" << std::endl;
      } else {
        if (typeA < typeB) {
          out << "    return -Compare < "; templateArg(typeB,'b'); out << ","; templateArg(typeA,'a'); out << " >::between(b,a);" << std::endl;
        } else {
          out << "    return "; testString(typeA,'a'); out << ".compare("; testString(typeB,'b'); out << ");" << std::endl;
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

  mk.out << "#include <stdint.h>" << std::endl;    
  if (includeFlash) {
    mk.out << "#include \"Flash.h\"" << std::endl;
  }
  mk.out << "#include \"ArduinoUnitWiden.h\"" << std::endl;  
  if (includeString) {
    mk.out << "#if defined(ARDUINO)" << std::endl;    
    mk.out << "#  include <WString.h>" << std::endl;
    mk.out << "#endif" << std::endl;        
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
        std::cerr << "phase " << mk.phase << ": " << mk.typeName(mk.typeA) << " vs " << mk.typeName(mk.typeB) << std::endl;
        mk.print();
      }
    }
  }

  static const char * versions [] = {"between","equal","notEqual","less","more","lessOrEqual","moreOrEqual"};
  for (auto version : versions) {
    std::string Version = version;
    Version[0] = version[0]+'A'-'a';
    const char *type = (Version == "Between") ? "int8_t" : "bool";
    mk.out << "template <typename A, typename B> " << type << " compare" << Version << "(const A &a, const B &b) { return Compare<typename ArduinoUnitWiden<A>::type,typename ArduinoUnitWiden<B>::type>::" << version << "(a,b); }" << std::endl;
    // mk.out << "template <typename A, typename B> " << type << " compare" << Version << "(const A &a, const B &b) { return Compare<A,B>::" << version << "(a,b); }" << std::endl;    
  }

  return 0;
}

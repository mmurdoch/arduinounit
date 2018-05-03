#if !defined(ARDUINO)

// only used for "en vitro" tests (not on actual board)

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string>
#include <vector>
#include <limits.h>

#include "ArduinoUnit.h"
#include "ArduinoUnitMock.h"

#include <signal.h>
#include <setjmp.h>
jmp_buf sigJump;

// break out of main loop on signal
// for (ctrl-c/INT or kill/TERM)
void sig(int num) { longjmp(sigJump, num); }

// simple impl of random
int random(int n) {
  static bool setup = false;
  if (!setup) {
    srand(time(0));
    setup = true;
  }
  return rand() % n;
}

int random(int a, int b) {
  return a+rand() % (b-a+1);
}

void setup();
void loop();

CppIOStream Serial;

struct Mock {
  double timeLimit;
  std::vector<std::pair<std::string,std::string>> selection;

  void args(int argc, const char *argv[]) {
    for (int i=1; i<argc; ++i) {
      if (strcmp(argv[i],"--exclude")==0 || strcmp(argv[i],"-e")==0) {
        ++i;
        selection.push_back(std::pair<std::string,std::string>("exclude",argv[i]));
        continue;
      }
      if (strcmp(argv[i],"--include")==0 || strcmp(argv[i],"-i")==0) {
        ++i;
        selection.push_back(std::pair<std::string,std::string>("include",argv[i]));
        continue;
      }
      if (strcmp(argv[i],"--time")==0 || strcmp(argv[i],"-t")==0) {
        ++i;
        timeLimit = atof(argv[i]);
        continue;
      }
      if (strcmp(argv[i],"--")==0) { break; }
      std::cerr << "unknown argument '" << argv[i] << "'" << std::endl;
      exit(1);
    }
  }

  void run() {
    setup();
    
    for (auto select : selection) {
      if (select.first == "exclude") {
        Test::exclude(select.second.c_str());
      } else if (select.first == "include") {
        Test::include(select.second.c_str());            
      }
    }
    
    // loop until tests complete 
    while (Test::remaining() > 0) {
      loop();
    }
  }

};  

int main(int argc, const char *argv[]) {
  Mock mock;
  mock.args(argc,argv);
  
  signal(SIGINT,sig); // ctrl-c
  signal(SIGTERM,sig);// kill
  signal(SIGALRM,sig);// timeout
  
  {
    // C++ "finally" clause  
    struct Fin { ~Fin() { Test::abort(); } } fin; 
    
    // branch back here with signal ctrl-c/kill/alarm != 0  
      if (setjmp(sigJump) == 0) {
        if (mock.timeLimit > 0 && mock.timeLimit*1000000 < INT_MAX) {
          ualarm(mock.timeLimit*1000000.0,0);
        }
        mock.run();
      }
  }
  
  return Test::getCurrentFailed() > 0 ? 1 : 0;
}


#include "advanced.ino"

#endif

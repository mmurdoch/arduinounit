#line 2 "firmware.ino"
#include <ArduinoUnit.h>

//
// line number sanity check
//
#if __LINE__ != 7
#error __LINE__ value is incorrect!
#endif


char line[80];
bool configuring = true;
unsigned long time = 0;
unsigned long startTime = 0;
int phase = 0;

void motd()
{
  Serial.println(F("Test overall and phase should always pass."));
  Serial.println(F("Interactive tester commands:"));
  Serial.println(F("  include <pattern>"));
  Serial.println(F("  exclude <pattern>"));
  Serial.println(F("  min_verbosity <binary number>"));
  Serial.println(F("  max_verbosity <binary number>"));
  Serial.println(F("  run"));
  Serial.println(F("Note:"));
  Serial.println(F("  Just 'run' will (correctly) have 1 test fail and 1 skip."));
  Serial.println(F("  Enable 'newline' in the serial monitor to send commands."));
}

void getline()
{
  int c = 0;
  size_t i = 0;
  Serial.print(F("command> "));
  for (;;) {
    while (!Serial.available()) { }
    c=Serial.read();
    if (c <= 0 || c == '\r') { continue; }
    if (c == 127 /* delete  */) { 
      if (i > 0) { 
        Serial.print(F("\010 \010")); // erase char on terminal
        --i; 
      }
      continue;
    }
    if (c == '\n') break;
    Serial.print((char)c);
    if (i < sizeof(line)) line[i]=c;
    ++i;
  }
  line[i < sizeof(line) ? i : sizeof(line)-1]=0;
  Serial.println();
}

bool cmd(const char *s)
{
  int len = strlen(s);
  return (strncmp(line,s,len) == 0) &&
    (line[len] == ' ' || line[len] == 0);
}

char *arg()
{
  int i=0;
  while (line[i] != ' ' && line[i] != 0) ++i;
  while (line[i] == ' ') ++i;
  return line+i;
}

int binary(char *arg)
{
  int value = 0;
  while (*arg == '0' || *arg == '1') {
    value = 2*value + (*arg == '1');
    ++arg;
  }
  return value;
}

void process()
{
  if (cmd("include")) { 
    Test::include(arg()); 
    return; 
  }
  if (cmd("exclude")) { 
    Test::exclude(arg()); 
    return; 
  }
  if (cmd("max_verbosity")) { 
    Test::max_verbosity = binary(arg());
    return;
  }
  if (cmd("min_verbosity")) { 
    Test::min_verbosity = binary(arg());
    return;
  }
  if (cmd("run")) {
    configuring = false;
    return;
  }

  Serial.print("unknown command: '");
  Serial.print(line);
  Serial.print("'");
  Serial.println();
}

void setup()
{
  Serial.begin(9600);
  motd();
  while (configuring) {
    getline();
    process();
  }
  phase=0;

  Test::include("phase");
  Test::include("overall");
  startTime = millis();
}

void loop()
{
  time = millis()-startTime;
  Test::run();
}


// all boolean asserts

bool checkcase(bool x, int c)
{
  switch (c) {
  case 0: return x;
  case 1: return !x;
  }
  return false;
}

void testcase(bool x,int c)
{
  switch (c) {
  case 0: assertTrue(x); break;
  case 1: assertFalse(x); break;
  }
}

test(assert_bools)
{
  for (int c = 0; c < 2; ++c) {
    for (int x = 0; x <= 1; ++x) {
      bool b = (x != 0);
      testcase(b,c);
      if (checkcase(b,c)) {
        if (state != LOOPING) { fail(); return; }
      } else {
        if (state != DONE_FAIL) { fail(); return; }
      }
      state = LOOPING;
    }
  }
}

// all integer asserts

bool checkcase(int x, int y, int c)
{
  switch (c) {
  case 0: return x < y;
  case 1: return x <= y;
  case 2: return x == y;
  case 3: return x != y;
  case 4: return x >= y;
  case 5: return x > y;
  }
  return false;
}

void testcase(int x, int y, int c)
{
  switch (c) {
  case 0: assertLess(x,y); break;
  case 1: assertLessOrEqual(x,y); break;
  case 2: assertEqual(x,y); break;
  case 3: assertNotEqual(x,y); break;
  case 4: assertMoreOrEqual(x,y); break;
  case 5: assertMore(x,y); break;
  }
}

test(assert_ints)
{
  for (int c = 0; c < 6; ++c) {
    for (int x = -1; x <= 1; ++x) {
      for (int y = -1; y <= 1; ++y) {
        testcase(x,y,c);
        if (checkcase(x,y,c)) {
          if (state != LOOPING) { fail(); return; }
        } else {
          if (state != DONE_FAIL) { fail(); return; }
        }
        state = LOOPING;
      }
    }
  }
}


// all string (const char *) asserts

const char * strings [] = { "", "a", "aa", "b", 0 };

bool checkcase(const char *x, const char *y, int c)
{
  switch (c) {
  case 0: return strcmp(x,y) < 0;
  case 1: return strcmp(x,y) <= 0;
  case 2: return strcmp(x,y) == 0;
  case 3: return strcmp(x,y) != 0;
  case 4: return strcmp(x,y) >= 0;
  case 5: return strcmp(x,y) > 0;
  }
  return false;
}

void testcase(const char *x, const char *y, int c)
{
  switch (c) {
  case 0: assertLess(x,y); break;
  case 1: assertLessOrEqual(x,y); break;
  case 2: assertEqual(x,y); break;
  case 3: assertNotEqual(x,y); break;
  case 4: assertMoreOrEqual(x,y); break;
  case 5: assertMore(x,y); break;
  }
}

test(assert_strings)
{
  for (int c = 0; c < 6; ++c) {
    for (int x = 0; strings[x] != 0; ++x) {
      for (int y = 0; strings[y] != 0; ++y) {
        testcase(strings[x],strings[y],c);
        if (checkcase(strings[x],strings[y],c)) {
          if (state != LOOPING) { fail(); return; }
        } else {
          if (state != DONE_FAIL) { fail(); return; }
        }
        state = LOOPING;
      }
    }
  }
}

// simple ongoing tests:

testing(passes)
{
  if (phase == 2 && time > 250) {
    pass();
  }
}

testing(fails)
{
  if (phase == 2 && time > 500) {
    fail();
  }
}

testing(skips)
{
  if (phase == 2 && time > 750) {
    skip();
  }
}

// meta-test checks

bool metacheck(int t, int c)
{
  switch (c) {
  case 0:
    switch(t) {
    case 0: return checkTestDone(passes);
    case 1: return checkTestDone(fails);
    case 2: return checkTestDone(skips);
    }
  case 1:
    switch(t) {
    case 0: return checkTestNotDone(passes);
    case 1: return checkTestNotDone(fails);
    case 2: return checkTestNotDone(skips);
    }
  case 2:
    switch(t) {
    case 0: return checkTestPass(passes);
    case 1: return checkTestPass(fails);
    case 2: return checkTestPass(skips);
    }
  case 3:
    switch(t) {
    case 0: return checkTestNotPass(passes);
    case 1: return checkTestNotPass(fails);
    case 2: return checkTestNotPass(skips);
    }
  case 4:
    switch(t) {
    case 0: return checkTestFail(passes);
    case 1: return checkTestFail(fails);
    case 2: return checkTestFail(skips);
    }
  case 5:
    switch(t) {
    case 0: return checkTestNotFail(passes);
    case 1: return checkTestNotFail(fails);
    case 2: return checkTestNotFail(skips);
    }
  case 6:
    switch(t) {
    case 0: return checkTestSkip(passes);
    case 1: return checkTestSkip(fails);
    case 2: return checkTestSkip(skips);
    }
  case 7:
    switch(t) {
    case 0: return checkTestNotSkip(passes);
    case 1: return checkTestNotSkip(fails);
    case 2: return checkTestNotSkip(skips);
    }
  }
  return false;
}

void metaassert(int t, int c)
{
  switch (c) {
  case 0:
    switch(t) {
    case 0: assertTestDone(passes); return;
    case 1: assertTestDone(fails); return;
    case 2: assertTestDone(skips); return;
    }
  case 1:
    switch(t) {
    case 0: assertTestNotDone(passes); return;
    case 1: assertTestNotDone(fails); return;
    case 2: assertTestNotDone(skips); return;
    }
  case 2:
    switch(t) {
    case 0: assertTestPass(passes); return;
    case 1: assertTestPass(fails); return;
    case 2: assertTestPass(skips); return;
    }
  case 3:
    switch(t) {
    case 0: assertTestNotPass(passes); return;
    case 1: assertTestNotPass(fails); return;
    case 2: assertTestNotPass(skips); return;
    }
  case 4:
    switch(t) {
    case 0: assertTestFail(passes); return;
    case 1: assertTestFail(fails); return;
    case 2: assertTestFail(skips); return;
    }
  case 5:
    switch(t) {
    case 0: assertTestNotFail(passes); return;
    case 1: assertTestNotFail(fails); return;
    case 2: assertTestNotFail(skips); return;
    }
  case 6:
    switch(t) {
    case 0: assertTestSkip(passes); return;
    case 1: assertTestSkip(fails); return;
    case 2: assertTestSkip(skips); return;
    }
  case 7:
    switch(t) {
    case 0: assertTestNotSkip(passes); return;
    case 1: assertTestNotSkip(fails); return;
    case 2: assertTestNotSkip(skips); return;
    }
  }
}

int metaNextPhase = 1;
testing(meta)
{
  if (phase == metaNextPhase) {
    for (int t=0; t<3; ++t) {
      for (int c=0; c<8; ++c) {
        metaassert(t,c);
        if (metacheck(t,c)) {
          if (state != LOOPING) { fail(); return; }
        } else {
          if (state != DONE_FAIL) { fail(); return; }
        }
        state = LOOPING;
      }
    }
    if (phase == 1) metaNextPhase = 3;
    else pass();
  }
}

testing(phase)
{
  bool done = true;
  switch(phase) {
  case 0:
    done = done && checkTestDone(assert_bools);
    done = done && checkTestDone(assert_ints);
    done = done && checkTestDone(assert_strings);
    break;
  case 1:
    done = done && (checkTestDone(meta) || (metaNextPhase == 3));
    break;
  case 2:
    done = done && (time > 1000);
    break;
  case 3:
    done = done && checkTestDone(meta);
    break;
  case 4:
    pass();
    break;
  }
  if (done) {
    ++phase;
    startTime = millis();
  }
}    

testing(overall)
{
  // wait until last...
  if (Test::getCurrentSkipped()+Test::getCurrentPassed()+Test::getCurrentFailed()+1 == Test::getCurrentCount()) {
    if (!checkTestSkip(passes)) assertTestPass(passes);
    if (!checkTestSkip(fails))  assertTestFail(fails);
    assertTestSkip(skips);
    if (!checkTestSkip(assert_bools)) assertTestPass(assert_bools);
    if (!checkTestSkip(assert_ints)) assertTestPass(assert_ints);
    if (!checkTestSkip(assert_strings)) assertTestPass(assert_strings);
    if (!checkTestSkip(meta)) assertTestPass(meta);
    pass();
  }
}

char *iss47a="47";
const char *iss47b="47";
char iss47c[3]={'4','7',0};
const char iss47d[3]={'4','7',0};
String iss47e="47";
const char iss47f_P[] PROGMEM  = {"47"};
const __FlashStringHelper *iss47f=(const __FlashStringHelper *)iss47f_P;

char *iss47A="48";
const char *iss47B="48";
char iss47C[3]={'4','8',0};
const char iss47D[3]={'4','8',0};
String iss47E="48";
const char iss47F_P[] PROGMEM  = {"48"};
const __FlashStringHelper *iss47F=(const __FlashStringHelper *)iss47F_P;

test(iss47)
{
  Serial.println("iss47");
  assertEqual(iss47a,iss47a);
  assertEqual(iss47a,iss47b);
  assertEqual(iss47a,iss47c);
  assertEqual(iss47a,iss47d);
  assertEqual(iss47a,iss47e);
  assertEqual(iss47a,iss47f);

  assertEqual(iss47b,iss47a);  
  assertEqual(iss47b,iss47b);
  assertEqual(iss47b,iss47c);
  assertEqual(iss47b,iss47d);
  assertEqual(iss47b,iss47e);
  assertEqual(iss47b,iss47f);  

  assertEqual(iss47c,iss47a);  
  assertEqual(iss47c,iss47b);  
  assertEqual(iss47c,iss47c);
  assertEqual(iss47c,iss47d);
  assertEqual(iss47c,iss47e);
  assertEqual(iss47c,iss47f);

  assertEqual(iss47d,iss47a);  
  assertEqual(iss47d,iss47b);  
  assertEqual(iss47d,iss47c);
  assertEqual(iss47d,iss47d);
  assertEqual(iss47d,iss47e);
  assertEqual(iss47d,iss47f);

  assertEqual(iss47e,iss47a);  
  assertEqual(iss47e,iss47b);  
  assertEqual(iss47e,iss47c);
  assertEqual(iss47e,iss47d);
  assertEqual(iss47e,iss47e);
  assertEqual(iss47e,iss47f);
  
  assertEqual(iss47f,iss47a);  
  assertEqual(iss47f,iss47b);  
  assertEqual(iss47f,iss47c);
  assertEqual(iss47f,iss47d);
  assertEqual(iss47f,iss47e);
  assertEqual(iss47f,iss47f);
  
  assertNotEqual(iss47a,iss47A);
  assertNotEqual(iss47a,iss47B);
  assertNotEqual(iss47a,iss47C);
  assertNotEqual(iss47a,iss47D);
  assertNotEqual(iss47a,iss47E);
  assertNotEqual(iss47a,iss47F);  

  assertNotEqual(iss47b,iss47A);  
  assertNotEqual(iss47b,iss47B);
  assertNotEqual(iss47b,iss47C);
  assertNotEqual(iss47b,iss47D);
  assertNotEqual(iss47b,iss47E);
  assertNotEqual(iss47b,iss47F);  

  assertNotEqual(iss47c,iss47A);  
  assertNotEqual(iss47c,iss47B);  
  assertNotEqual(iss47c,iss47C);
  assertNotEqual(iss47c,iss47D);
  assertNotEqual(iss47c,iss47E);
  assertNotEqual(iss47c,iss47F);  

  assertNotEqual(iss47d,iss47A);  
  assertNotEqual(iss47d,iss47B);  
  assertNotEqual(iss47d,iss47C);
  assertNotEqual(iss47d,iss47D);
  assertNotEqual(iss47d,iss47E);
  assertNotEqual(iss47d,iss47F);  

  assertNotEqual(iss47e,iss47A);  
  assertNotEqual(iss47e,iss47B);  
  assertNotEqual(iss47e,iss47C);
  assertNotEqual(iss47e,iss47D);
  assertNotEqual(iss47e,iss47E);
  assertNotEqual(iss47e,iss47F);  

  assertNotEqual(iss47f,iss47A);  
  assertNotEqual(iss47f,iss47B);  
  assertNotEqual(iss47f,iss47C);
  assertNotEqual(iss47f,iss47D);
  assertNotEqual(iss47f,iss47E);
  assertNotEqual(iss47f,iss47F);  
  
  assertNotEqual(iss47A,iss47a);
  assertNotEqual(iss47A,iss47b);
  assertNotEqual(iss47A,iss47c);
  assertNotEqual(iss47A,iss47d);
  assertNotEqual(iss47A,iss47e);
  assertNotEqual(iss47A,iss47f);  

  assertNotEqual(iss47B,iss47a);  
  assertNotEqual(iss47B,iss47b);
  assertNotEqual(iss47B,iss47c);
  assertNotEqual(iss47B,iss47d);
  assertNotEqual(iss47B,iss47e);
  assertNotEqual(iss47B,iss47f);  

  assertNotEqual(iss47C,iss47a);  
  assertNotEqual(iss47C,iss47b);  
  assertNotEqual(iss47C,iss47c);
  assertNotEqual(iss47C,iss47d);
  assertNotEqual(iss47C,iss47e);
  assertNotEqual(iss47C,iss47f);  

  assertNotEqual(iss47D,iss47a);  
  assertNotEqual(iss47D,iss47b);  
  assertNotEqual(iss47D,iss47c);
  assertNotEqual(iss47D,iss47d);
  assertNotEqual(iss47D,iss47e);
  assertNotEqual(iss47D,iss47f);  

  assertNotEqual(iss47E,iss47a);  
  assertNotEqual(iss47E,iss47b);  
  assertNotEqual(iss47E,iss47c);
  assertNotEqual(iss47E,iss47d);
  assertNotEqual(iss47E,iss47e);
  assertNotEqual(iss47E,iss47f);  

  assertNotEqual(iss47F,iss47a);  
  assertNotEqual(iss47F,iss47b);  
  assertNotEqual(iss47F,iss47c);
  assertNotEqual(iss47F,iss47d);
  assertNotEqual(iss47F,iss47e);
  assertNotEqual(iss47F,iss47f);  
}

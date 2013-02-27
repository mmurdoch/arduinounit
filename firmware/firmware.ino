#line 2 "firmware.ino"
#include <ArduinoUnit.h>

char line[80];
bool configuring = true;

void motd()
{
  Serial.println(F("The overall test should pass..."));
  Serial.println(F("interactive tester commands:"));
  Serial.println(F("  include <pattern>"));
  Serial.println(F("  exclude <pattern>"));
  Serial.println(F("  min_verbosity <binary number>"));
  Serial.println(F("  max_verbosity <binary number>"));
  Serial.println(F("  run"));
}

void getline()
{
  int c = 0;
  int i = 0;
  Serial.print(F("command> "));
  for (;;) {
    while (!Serial.available()) { }
    c=Serial.read();
    if (c <= 0) { continue; }
    if (c == 127 /* delete  */) { 
      if (i > 0) { 
	Serial.print(F("\010 \010")); // erase char on terminal
	--i; 
      }
      continue;
    }
    if (c == '\n' || c == '\r') break;
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
}

void loop()
{
  Test::run();
}

// simple ongoing tests:

testing(passes)
{
  if (millis() > 1000) {
    pass();
  }
}

testing(fails)
{
  if (millis() > 1000) {
    fail();
  }
}

testing(skips)
{
  if (millis() > 1000) {
    skip();
  }
}

// all boolean asserts

bool checkcase(bool x, int c)
{
  switch (c) {
  case 0: return x;
  case 1: return !x;
  }
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

testing(meta)
{
  static unsigned long timeout = 0;
  if (millis() >= timeout) {
    timeout = millis() + 500;
    
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

    if (timeout >= 2000) pass();
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

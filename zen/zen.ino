#include <ArduinoUnit.h>

// Pseudo-replacement for reset
// #include <SoftReset.h>
// To make this not depend on other lib
void soft_restart() {
  Serial.println("press reset.");
  while (true) { } // 
}


// Test All The Things
test(flash) { /* .. */ }
test(lcd) { /* .. */ }
testing(battery) { /* ..  */ }
testing(heaterRelay) { /* ..  */ }
testing(heaterTempSense) { /* .. */ }
testing(chillerTempSense) { /* .. */ }
testing(hotBinTemp) { /* .. */ }
testing(coldBinTemp) { /* .. */ }

const int idiotLightPin = 13;

void idiotLightSetup() {
    pinMode(idiotLightPin, OUTPUT);
    digitalWrite(idiotLightPin, LOW);
}

void idiotLightOn() {
  digitalWrite(idiotLightPin,HIGH);
}

void idiotLightOff() {
  digitalWrite(idiotLightPin,LOW);
}

void idiotLightFlip() {
  digitalWrite(idiotLightPin,
               ! digitalRead(idiotLightPin));
}

void idiotLightLoop() {
  static uint32_t next = 0;
  if (Test::getCurrentFailed() > 0
      && (next == 0 || int32_t(millis()-next) >= 0)) {
    next=millis()+1000;
    idiotLightFlip();
  }
}

// alphabetically last test
// (declaration order does not matter)
test(zzzz_powerOnSelfTest) {
  assertTestPass(flash);
  assertTestPass(lcd);  
}

void powerOnSelfTest() {
  uint8_t saveVerbosity = Test::min_verbosity;

  // get reports on pass/fail & skipped tests...
  Test::min_verbosity =
    TEST_VERBOSITY_TESTS_ALL | TEST_VERBOSITY_ASSERTIONS_FAILED;

  // run tests until the post test is complete
  while (!checkTestDone(zzzz_powerOnSelfTest)) {
    Test::run();
  }

  // halt if post fails
  if (checkTestFail(zzzz_powerOnSelfTest)) {
    while (true) {
      idiotLightFlip();
      delay(1000);
    }
  }

  // restore original min_verbosity
  Test::min_verbosity = saveVerbosity;
}

bool criticalError = false;

testing(sanity) {
  assertFalse(criticalError);
  assertTestNotFail(battery);
  assertTestNotFail(heaterRelay);
  assertTestNotFail(heaterTempSense);
  assertTestNotFail(chillerTempSense);  
}

void sanityCheck() {
  if (checkTestFail(sanity)) {
    for (int i=5; i>0; --i) {
      idiotLightFlip();
      delay(100*i);
    }
    soft_restart();
  }
}

void testSetup(Stream &testStream) {
  idiotLightOn();
  Test::out = &testStream;


  // exclude Slow tests, unless they are Critical
  Test::exclude("*Slow*");
  Test::include("*Critical*");

  powerOnSelfTest();
  sanityCheck();
  idiotLightOff();  
}

// run repeated tests & sanity check every interval ms

void testLoop() {
  const uint16_t interval = 250;
  static uint32_t next = interval;
  
  if (int32_t(micros()-next) < 0) {
    return;
  }

  next += interval;
  Test::run();
  sanityCheck();
}

void setup() {
  Serial.begin(115200L);
  while (!Serial) {} // Leonardo/Due Mantra

  idiotLightSetup();

  /* other setup */

  testSetup(Serial);
}

void loop() {
  idiotLightLoop();  
  /* other loop */
  testLoop();
}


const double Kfreeze=273.15;
const double Cfreeze=0.0;
const double Kboil=373.15;
const double Cboil=100.0;
const double KabsZero=0.0;
const double CabsZero=-273.15;

// example of testing input to function
double KtoC(double K) {
    // input assert with non-void return value
  
    assertMoreOrEqual(K,KabsZero,F("below absolute zero"),CabsZero);
    double C = K-Kfreeze;
    return C;
}

// example of testing of function
test(KtoC) {
  const double tolerance=0.0001;
  assertNear(KtoC(KabsZero),CabsZero,tolerance);
  assertNear(KtoC(Kfreeze),Cfreeze,tolerance);
  assertNear(KtoC(Kboil),Cboil,tolerance);  
}

// example of testing output from function (non default return)
double CtoK(double C) {
  double K = C+Kfreeze;
  const double absZeroK = 0;
  assertMoreOrEqual(K,absZeroK,F("below absolute zero"),absZeroK);
  return K;
}

void critical() {
     criticalError = true;
}

void io() {
     idiotLightFlip();
}

int intReturnExample() {
  assertEqual(1,1,"weird",(critical(),io(),0));
  return 1;
}

void voidReturnExample() {
  assertEqual(1,1,"still weird",(critical(),io()));
}

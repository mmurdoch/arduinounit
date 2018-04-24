// This is for compile time tests.
//
// You would have use this in some context to be useful.

#include <ArduinoUnit.h>
// optional library -- see soft_restart() below
// #include <SoftReset.h>
#include <float.h>
#include <math.h>

// everyone's idea of equal for floats/doubles is a little different...
const double deps = sqrt(DBL_EPSILON);
#define assertClose(a,b) { \
    double dlhs = (a); \
    double drhs =(b); \
    double derr = fabs(dlhs-drhs); \
    assertLess(derr,deps,(#a "=") << dlhs << " ~ " << (#b "=") << drhs); \
}

test(WaterSensorConnected) { /* ... */ }
testing(WaterSensorSane) { /* ... */ }
test(FlashChecksum) { /* .. */ }
test(RelayCycle) { /* .. */ }

test(zzzz_post) {
  assertTestPass(FlashChecksum);
  assertTestPass(RelayCycle);
}

void TestSetup() {
    /* First Test::run() calls setup() of each test.
       These usually do nothing, but may be customized
       for advanced tests.  Include/exclude tests *before* run */
    Test::run();
}


void TestLoop() {
    static uint32_t nextTest = 0;
    if (int32_t(micros()-nextTest) >= 0) {
        nextTest += 250; // run tests every 250 ms;
        Test::run();
    }
}

void SerialSetup() {
    Serial.begin(9600);

    /* Leonardo needs this, and it does no harm... */
    while (!Serial) {}
}

const int IdiotLightPin = 13;

void IdiotSetup() {
    pinMode(IdiotLightPin, OUTPUT);
    digitalWrite(IdiotLightPin, LOW);
}

void IdiotOn() {
    digitalWrite(IdiotLightPin,HIGH);
}

/* on->off->on ... */
void IdiotFlip() {
    digitalWrite(IdiotLightPin,
        ! digitalRead(IdiotLightPin));
}

/* flash 1/sec if tests fail */
void IdiotLoop() {
    static uint32_t nextFlip = 0;
    if (Test::getCurrentFailed() > 0
        && int32_t(millis()-nextFlip) >= 0) {
        nextFlip=millis()+1000;
        IdiotFlip();
    }
}

/* tests are run alphabetically, so this is the last test (little z is after big Z, so make them small) */

void PowerOnSelfTest() {
    while (!checkTestDone(zzzz_post)) {
        Test::run();
    }
    if (!checkTestPass(zzzz_post)) {
        while (true) {
            IdiotFlip();
            delay(250);
        }
    }
}

float BatteryVoltage() { return 2.5; }

/* repeated until pass or fail */
testing(BatteryCritical) {
    assertMore(BatteryVoltage(),3.0);
}

uint32_t NetworkLastPacketTime() { return 0; }

/* repeated until pass or fail */
testing(NetworkLost) {
    assertLess(int32_t(millis()-NetworkLastPacketTime()), 10000);
}

testing(Sanity) {
    assertTestNotFail(BatteryCritical);
    assertTestNotFail(NetworkLost);
}

void SanityLoop() {
    if (checkTestFail(Sanity)) {
        for (int i=5; i>0; --i) {
            IdiotFlip();
            delay(100*i);
        }
        IdiotOn();
        while (true) {}
        // soft_restart();
    }
}

const char *IO(bool ok) {
    if (!ok) IdiotFlip();
    return "io";
}

int16_t CrcPacket() { return 0; }
int16_t CrcCompute() { return 1; }
void ProcessPacket() {}
void NextPacket() {
    assertEqual(CrcPacket(), CrcCompute(),IO(ok));
    ProcessPacket();
}

int InInt(const char *name, Stream &io=Serial) {
    io.print("integer ");
    io.print(name);
    io.print("? ");
    return io.parseInt();
}

test(InInt) {
    MockStream ms;
    int xsend = 132;
    ms.input.println(xsend);
    int xrecv = InInt("x",ms);
    assertEqual(xsend,xrecv);
    assertEqual(ms.output,"integer x? ");
}

void UseInInt() {
    int a = InInt("a");
    int b = InInt("b");
    (void) a;
    (void) b;
    /* ... */
}

void TimeCritical() {
    const int32_t TooLongMicros = 10000;
    uint32_t start = micros();
    /* ... */
    assertLess(int32_t(micros()-start),TooLongMicros);
}

void SpaceCritical() {
    const int16_t SpaceRequired = 350;
    assertMore(freeMemory(), SpaceRequired);
    /* ... */
}

double F;
const int TempPin = 0;

void ReadTempBad() {
    double K = analogRead(TempPin)*1024;
    double C = K-273;
    F=(9/5)*C-32;
}


double TempRaw() { // rule
    return analogRead(TempPin);
}

double TempRaw2K(double raw) { // serve
    return raw*1024;
}

double TempK2C(double K) { // serve
    return K - 273;
}

double TempC2F(double C) { // serve
    return (9/5)*C-32;
}

double TempRaw2F(double raw) { // serve
    double K = TempRaw2K(raw);
    double C = TempK2C(K);
    return TempC2F(C);
}

void ReadTempGood() { // rule
    float raw = TempRaw();
    F = TempRaw2F(raw);
}

const int MinTempRaw = 100;
const int MaxTempRaw = 900;

testing(TempRaw) {
    double raw = TempRaw();
    assertLessOrEqual(MinTempRaw,raw);
    assertLessOrEqual(raw, MaxTempRaw);
}

test(TempRaw2K) {
    assertClose(TempRaw2K(MinTempRaw),0.0);
    assertClose(TempRaw2K(MaxTempRaw),1000.0);
}

test(TempK2C) {
    assertClose(TempK2C(0.0),-273.15);
    assertClose(TempK2C(273.15),0.0);

}

test(TempC2F) {
    assertClose(TempC2F(0.0),32.0);
    assertClose(TempC2F(100.0),212.0);
}

void setup() {
    SerialSetup();
    TestSetup();
    IdiotSetup();
    PowerOnSelfTest();
    /* ... */
}

void loop() {
    TestLoop();
    IdiotLoop();
    /* ... */
}

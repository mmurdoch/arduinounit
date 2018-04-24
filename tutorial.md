# ArduinoUnit Mini Tutorial

Building reliable embedded software is a rewarding experience.  Some important challenges are:

* Resources are limited.  Where a typical developer can use a pre-built library, an embedded developer might have to craft a specialized tool.

* Systems are dark.  Larger systems can log everything.  Embedded devices often can just keep up with what they need to do, and spending resources logging events can break what they do.  Stepping through with a debugger may be impossible, or just hard because of all the real-time events the device interacts with.

* Hardware is specific.  A real time clock that drives an interrupt on an 8-bit micro-controller which is controlling a coil that drives a metronome who's position is measured by an analog hall-effect sensor has many points of failure that is not easy to identify as "software".  But are part of the system and can cause the system to fail none the less.

* Time and money are short.  Projects are almost universally wanted quickly and cheaply.

ArduinoUnit does not instantly change any of these.  But

* ArduinoUnit is designed to be small so it can work in tiny systems.
* Tests can illuminate what does and does not work. ArduinoUnit does this with as little as a serial port.  ArduinoUnit can also run "en vitro" on the development environment, where it is easier to discover what is wrong.
* ArduinoUnit was originally designed to run "en vivo" on the actual embedded hardware, where any system specific components can be tested and used.  This means your tests can include any specific idiosyncrasies about your embedded design.
* Testing saves time and money (and stress).  It is really the only possibility for delivering reliable solutions to complex problems.
* ArduinoUnit is MIT licensed, so it is free for both open and closed-source applications with no viral clauses to make any lawyers you know nervous.

## Why

Writing software can be hard.  Why make it harder by adding tests to the problem:

    Writing incorrect code is easy, but useless.

And

    Writing correct code __without__ tests is really hard.

Software for embedded systems is challenging, so bringing your best game is a good idea. It also builds skills you can use in all your software walks of life.

## What

What kinds of tests are useful?

* __Configuration__ Is some important startup requirement missing?  (Like the BIOS Power On Self Test)
* __Status__ Are there device/component failures? (Like a Low battery indicator)
* __Input__ Are there input failures? (Like missing/corrupted messages, or bad parameters for a function value)
* __Output__ Are there output failures? Are results of functions within bounds (Like a Kelvin temperature must be positive), or formats correct?
* __State__ Is the current state sane? (Does the IMU think we are traveling slower than the speed of light)
* __Time__ Is something taking too long to compute?
* __Space__ Is something taking too much room to store?

## Test All The Things

Imagine you are running a hot dog stand.  Now imagine trying to do it on a cart that will only roll forward and left, the heater takes 30 minutes to start, the chiller leaks water, and your money box only opens 80% of the time.  You will be out of business soon.

A little math is useful here.  If you have N independent components, each with reliability P, then the chance all of them work together is Q=P^N.  If your system has N=10 parts with P=0.9=90% reliability, then the chance it will all work is Q=P^N=(0.9)^10=0.35=35%.  Not a bridge I want to use.

Turns out, if you wanted a Q=99% reliability of a system with N=10 independent components, each component has to be P=Q^(1/N) = (0.99)^(0.1) = 0.999 = 99.9% reliable.

    Solving bigger problems reliably requires more reliable components

Equate "Thing" and "Unit" in your mind.  We solve complicated problems by breaking them up into parts, and then putting those parts together to make bigger parts.  Each part is a "unit" that should be tested to make sure it works; so you can reliably use it as a part in a larger unit.

__A reliable thing works when in good repair and used correctly, and has detectable error status when it is broken or misused.__

## Some Religion

Ideally, a unit test will test the suitability of a component for any situation (a bolt should work, no matter what parts they are holding together).  Of course real bolts don't always work because of subtleties of the application (heat, cold, vibration, etc).

ArduinoUnit is a non-denominational testing framework: you can use it to make highly independent tests of each component, but you can also build tests that are application specific. Building reliable software requires both.  After all, your entire embedded system will likely be a "unit" in someone else's design.

## Making a reliable thing

### Step 1: The Idiot Light

Pejoratives aside, this idea is really useful and we are all idiots most of the time (there are infinitely more things we don't know than we know).  It is important to have a thumbs up/thumbs down status so we can at least decide to look more closely.

```c++
#include <ArduinoUnit.h>

test(WaterSensorConnected) { /* ... */ }

testing(WaterSensorSane) { /* ... */ }

void TestSetup() {
    /* First Test::run() calls setup() of each test.  These usually do nothing, but may be customized for advanced tests */
    Test::run();
}

void TestLoop() {
    Test::run();
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

void setup() {
    SerialSetup();
    TestSetup();
    IdiotSetup();
    /* ... */
}

void loop() {
    TestLoop();
    IdiotLoop();
    /* ... */
}
```

If all your sensors are disconnected but you want to test other things.  You can exclude sensor tests (hopefully temporarily) with

```c++
void TestSetup() {
    Test::exclude("*Sensor*");
    Test::run();
}
```

If you __only__ wanted to test sensors, then:

```c++
void TestSetup() {
    Test::exclude("*");
    Test::include("*Sensor*");
    Test::run();
}
```

### Step 2: The Power On Self Test

If your thing can't run without certain things, you should make sure those certain things are ok or just stop. Add the following:

```c++
test(FlashChecksum) { /* .. */ }
test(RelayCycle) { /* .. */ }

/* tests are run alphabetically, so this is the last test (little z is after big Z, so make them small) */
test(zzzz_post) {
  assertTestPass(FlashChecksum);
  assertTestPass(RelayCycle);
}

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
```

Then call `PowerOnSelfTest()` as the last step of your setup.

### Step 3: Sanity

The world can be a wild and wooly place. Your system should revert to some safe state if it can't make sense of it. This uses another library, SoftReset, to reboot the Arduino in the hopes it gets better.

```c++
/* addtional library to reset the arduino */
#include <SoftReset.h>

/* repeated until pass or fail */
testing(BatteryCritical) {
    assertMore(BatteryVoltage(),3.0);
}

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
        soft_restart();
    }
}
```

Add `SanityLoop()` to your loop to stay sane or die trying.

### Step 4: IO

There is a chicken-and-egg problem with I/O.  If you have input and output errors, how can you tell there are problems?  You can hijack assert footnotes to give a hint by flickering the idiot light:

```c++
const char *IO(bool ok) {
    if (!ok) IdiotFlip();
    return "io";
}

void NextPacket() {
    assertEqual(CrcPacket(), CrcCompute(),IO(ok));
    ProcessPacket();
}
```

By passing a stream reference to IO operations, you can test if specific input and output steps work using the MockStream to simulate a serial port.  It is nice to set the default to the actual destination, so you don't have to type it everywhere:

```c++
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
    /* ... */
}

```

### Step 5: Time

It is important to know if something takes too long.  The following accounts for your system running long enough to suffer from the 32-bit microsecond counter rolling back through zero.

```c++
void TimeCritical() {
    const int32_t TooLongMicros = 10000;
    uint32_t start = micros();
    /* ... */
    assertLess(int32_t(micros()-start),TooLongMicros);
}
```

### Step 6: Space

Recursion (functions that directly or indirectly use themselves to solve a reduced version of a problem) and dynamic memory (malloc or new operator, or dynamic structures like String that use malloc/new) create systems that just run out of room.

```c++
void SpaceCritical() {
    const int16_t SpaceRequired = 350;
    assertMore(freeMemory(), SpaceRequired);
    /* ... */
}
```

## Meditations on The Heisenberg Uncertainty Principle

In physics, the Heisenberg Uncertainty Principle makes a specific statement about how well you can know where something is at (position) and simultaneously how fast it is moving (velocity). For small particles, this has to do with the impossibility of discovering where something is at without throwing something at it, which of course makes it move.

Stepping away from the finding bowling pins with bowling balls problem, there is a little HUP in many things.

Measuring something often changes it, and tests are a kind of observation of software. Now it is true that writing tests can change how an embedded system runs, and you may have to be careful when you spend time and resources testing so there is enough left to solve your problem.

But.

More importantly, writing tests changes how you write code.  Tests __observe__ code, and good tests are so valuable that you will want to adjust your habits so you code is easier to observe (hopefully operating correctly) with tests.

### Serve, don't rule

Code that insists on doing things one way is hard to test.  For example:

```c++


double F;
const int TempPin = 0;
void ReadTempF() {
    double K = analogRead(TempPin)*1024;
    double C = K-273;
    F=(9/5)*C-32;
}
```

`ReadTempF` is very insistent; marching from the activation of the analog-to-digital converter, to a specific sequence of transformations resulting in what we hope is the Fahrenheit temperature. It is incorrect (most code is initially), but there is little to do discover what part(s) are wrong.

Consider however:

```c++
#include <float.h>
#include <math.h>

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

float F;

void ReadTemp() { // rule
    float raw = TempRaw();
    F = TempRaw2F(raw);
}

// everyone's idea of equal for floats/doubles is a little different...
const double deps = sqrt(DBL_EPSILON);
#define assertClose(a,b) { \
    double dlhs = (a); \
    double drhs =(b); \
    double derr = fabs(dlhs-drhs); \
    assertLess(derr,deps,(#a "=") << dlhs << " ~ " << (#b "=") << drhs); \
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
```

Ok, so the first and last parts `TempRaw` and  `ReadTemp` are still "rule", since they need to access specific things (the analog sensor and the global temperature F).  But all the parts are separated and the parts that can serve many purposes can be well tested.  Once the components are tested, it is easier to rely on them for the overall solution.  A good compiler will turn them both into the same code as well.

### Slow and Steady Wins the Race

Solve problems in little parts you can test. Then test the parts put together. Eventually you have a working hot dog cart.

Eventually you will be turning your hot dog cart into a taco stand.  Trust me everybody does. So leave in all the little parts and the little tests. This makes changing things much easier.

### A Journey of 1000 Miles Begins With A Map

Planning every detail ahead of time is a waste of effort. So is just charging into the woods. In software you build the map with tests. If you don't know how to make a thing work, try writing the tests that should pass when it does work. The tests will help solidify what the Thing really is, including a lot about what it does not have to be.

### Your Momma Writes Better Tests Than You

You cannot see past your own design. Have other people suggest what needs to be tested. Have other people test it. Take their advice; your mother is usually right.

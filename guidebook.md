# ArduinoUnit Guidebook: The Zen of Testing

## Who

If you are already familiar with testing, I suggest you skip to __Making A Reliable Thing__, which addresses specific details about using ArduinoUnit and some suggested patterns of using it to solve common problems in embedded system design.

If you are a total beginner, welcome!  There are some entry requirements, however. I do expect that you have worked with Arduino's at least a little, and have solved some problem with it (perhaps as a school assignment, or just because you like this kind of stuff).

If you don't know about `setup()` and `loop()` and `pinMode()`, then get your green belt first.  Seek out some tutorials on using Arduino and Arduino-like systems elsewhere. Wax on. Wax off.

If you have stared at a little Arduino board you are nurturing to life and wondered

    What is going on in there, why isn't the servo moving?

then you are our kind of people.

## Why

Building reliable embedded software is a rewarding experience.  Some important challenges are:

* Resources are limited.  Where a typical developer can use a pre-built library, an embedded developer might have to craft a specialized tool.

* Systems are dark.  Larger systems can log everything.  Embedded devices often can just keep up with what they need to do, and spending resources logging events can break what they do.  Stepping through with a debugger may be impossible, or just hard because of all the real-time events the device interacts with.

* Hardware is specific.  A real time clock that drives an interrupt on an 8-bit micro-controller which is controlling a coil that drives a metronome who's position is measured by an analog hall-effect sensor has many points of failure that is not easy to identify as "software".  But are part of the system and can cause the system to fail none the less. [If you think there is a difference between hardware and software, explain why all the transistors your "software" runs on is different philosophically from a hall effect sensor.  (Answer: you may have been trained to ignore one of them)]

* Time and money are short.  Projects are almost universally wanted quickly and cheaply.

ArduinoUnit does not instantly change any of these.  But

* ArduinoUnit is designed to be small so it can work in tiny systems.
* Tests can show what does and does not work. ArduinoUnit does this with as little as a serial port.  ArduinoUnit can also run "en vitro" on the development environment, where it is easier to discover what is wrong.
* ArduinoUnit was originally designed to run "en vivo" on the actual embedded hardware, where any system specific components can be tested and used.  This means your tests can include any specific idiosyncrasies about your embedded design.
* Testing saves time and money (and stress).  It is really the only possibility for delivering reliable solutions to complex problems.
* ArduinoUnit is MIT licensed, so it is free for both open and closed-source applications with no viral clauses to make any lawyers you know nervous.

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

    Solving bigger problems reliably requires more reliable components.

Equate "Thing" and "Unit" in your mind.  We solve complicated problems by breaking them up into parts, and then putting those parts together to make bigger parts.  Each part is a "unit" that should be tested to make sure it works; so you can reliably use it as a part in a larger unit.

__A reliable thing works when in good repair and used correctly, and has detectable error status when it is broken or misused.__

## Some Religion

Ideally, a unit test will test the suitability of a component for any situation (a bolt should work, no matter what parts they are holding together).  Of course real bolts don't always work because of subtleties of the application (heat, cold, vibration, etc).

ArduinoUnit is a non-denominational testing framework: you can use it to make highly independent tests of each component, but you can also build tests that are application specific. Building reliable software requires both.  After all, your entire embedded system will likely be a "unit" in someone else's design.

## How to Make a Reliable Thing

### Step 0: ArduinoUnit basics

Add the library (Sketch->Include Library->Manage Libraries...), find the "basic" example and upload it.  Open your serial monitor (9600 baud and you should see some words about tests).  Look at the code a bit...

#### Serial Port

* ArduinoUnit uses `Serial` by default for reporting.  Don't forget to set it up [`Serial.begin(baud)`], or point it elsewhere [`Test::out = &Serial3` and `Serial3.begin(baud)`] in your `setup()`.  Remember to match the baud rate when looking at the serial monitor.

#### `test/ing()`

* `test(thingFor) {...}` creates a test named `thingFor` that will be executed once.
* `testing(thingFor) {...}` creates a test named `thingFor` that will be executed repeatedly.
* `thingFor` can be replaced with any combination of letters, numbers, and underscores (_) but __no spaces__ and must be __unique__ among test/ing().  Good names might be `batteryLevel` and `messageSentOnTime`.
* All active test & testing blocks are executed in alphabetical order.  If you want to control test order you can use test names like `000_BatteryLevel` and `010_MessageSentOnTime`.
* By default, if a test finishes, it is a `pass()`.  Usually, tests have assertions (discussed next) which (if they fail) have it finish early with a `fail()` status.
* Continuous `testing()` tests are called again and again [usually from your `loop()`].  Forever if that is your thing.  You can end them by calling `pass()` or `fail()` directly or having an assertion fail.
* If all the tests complete, a summary is printed.

In a test block `{ ... }` you can put code.  Any code really, but some particularly useful code is

#### Verbosity

* `verbosity = TEST_VERBOSITY_ALL` to see everything, pass or fail.

#### `pass()` or `fail()`

* `pass()` or `fail()` mark this test as passed or failed.  The current test will continue to the end (which may change it's mind), but it will be resolved. This means a `testing()` environment will not loop again.

#### Assertions

* `assert[Relation](a,b[,foot<<note[,retval]])` or `assertTest[Status](thingFor[,foot<<note[,retval]])`

  * `[Relation]` is one of: `Equal`, `NotEqual`, `Less`, `More`, `LessOrEqual`, `MoreOrEqual`.
  * `[Status]` is one of: `Done`, `Pass`, `Skip`, `Fail`, `NotDone`, `NotPass`, `NotSkip`, `NotFail`.
  * `thingFor` is some test/testing name.
  * The `<<` in the optional `foot<<note` separates things you can print.
  * The optional `retval` is to control the return value (normally nothing) when an assertion fails.

* For float and double values, `assertNear(a,b,max [,foot << note])` tests `|b-a|<=max`.  If you are working with very large or very small numbers, use `assertRelativelyNear`, which divides the error by the average magnitude, `½(|a|+|b|)`.  Floating point arithmetic is almost never exact so don't expect them to be `Equal`.

* `checkTest[Status](thingFor)` Just true/false depending on the current status of `test/ing(thingFor)`.

The asserts are replaced with code like:

    if (not assertion) { fail(); return retval; }

But also print out a status message (by default only if the assertion fails). For example:

    assertEqual(a[i],b[j],"i="<<i<<",j="<<j);

will generate a message like:

    Assertion failed: (a[i]=3) == (b[j]=4), file sketch.ino, line 17 [i=0,j=2].

With a few tests and asserts in place, you can control them in your `setup()` and `loop()`.

Simply:

```c++
#include <ArduinoUnit.h>

test(numbers) { assertNotEqual(1,2); }

void setup() {
    Serial.begin(9600);
    while (!Serial) {} // Leonardo Serial Mantra
}

void loop() {
    Test::run();
}
```

Tests that are completed are removed from the list (this is not a dynamic memory thing, trust me), so only the active tests are executed.  This means you can have a lot of tests that complete quickly and they represent no run-time overhead once they are resolved.

### Step 1: The Idiot Light

Pejoratives aside, this idea is really useful and we are all idiots most of the time (there are infinitely more things we don't know than we know).  It is important to have a thumbs up/thumbs down status so we can at least decide to look more closely.  If you or your manager puckers when they see your code, replace `idiot` with `status`.  We know what you mean: "Toilet" vs. "Bathroom", it's all the same business.

So let us imagine we are building a controller for a food cart.  I'm not going to bother with the implementation, just the tests.  By default if a test completes it is a `pass()`.  You can put a fail or some simple assertions to see how the output changes.
```c++
#include <ArduinoUnit.h>

test(flash) { /* .. */ }
test(lcd) { /* .. */ }
testing(battery) { /* ..  */ }
testing(heaterRelay) { /* ..  */ }
testing(heaterTempSense) { /* .. */ }
testing(chillerTempSense) { /* .. */ }
testing(hotBinTemp) { /* .. */ }
testing(coldBinTemp) { /* .. */ }

const int idiotLightPin = 13;

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

void idiotLightSetup() {
    pinMode(idiotLightPin, OUTPUT);
    digitalWrite(idiotLightPin, LOW);
}

//
// blink idiot light 1/sec if any tests fail
//
void idiotLightLoop() {
  static uint32_t next = 0;
  if (Test::getCurrentFailed() > 0
      && (next == 0 || int32_t(millis()-next) >= 0)) {
    next=millis()+1000;
    idiotLightFlip();
  }
}

void testSetup() {
  idiotLightOn();

  // first run sets up tests (usually nothing)
  Test::run();

  idiotLightOff();
}

void testLoop() {
  Test::run();
}


void setup() {
  Serial.begin(115200L);
  while (!Serial) {} // Leonardo/Due Mantra

  idiotLightSetup();
  
  // ...
  
  testSetup();
}

void loop() {
  idiotLightLoop();
  
  // ...
  
  testLoop();
}
```

If all your sensors are disconnected but you want to test other things.  You can exclude sensor tests (hopefully temporarily) with

```c++
void testSetup() {
    Test::exclude("*Sense");
    Test::run();
}
```

If you __only__ wanted to test sensors, then:

```c++
void testSetup() {
    Test::exclude("*");
    Test::include("*Sense");
    Test::run();
}
```

### Step 2: The Power On Self Test

If your thing can't run without certain things, you should make sure those certain things are ok or just stop. Add the following:

```c++
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
```

Then call `powerOnSelfTest()` as the last step of your test setup:

```c++
void testSetup() {
  idiotLightOn();

  // include/exclude ...

  powerOnSelfTest();

  idiotLightOff();
}
```

### Step 3: Sanity

The world can be a wild and wooly place. Your system should revert to some safe state if it can't make sense of it. This uses another library, SoftReset, to reboot the Arduino in the hopes it gets better.

Add the "SoftReset" library to your IDE and include it in your project:

```c++
#include <ArduinoUnit.h>
#include <SoftReset.h>
```

Add a repeating sanity test:

```c++
testing(sanity) {
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
```

Add `sanityCheck()` to your testLoop():

```c++
void testLoop() {
  Test::run();
  sanityCheck();
}
```

At this point, you might be testing too often [every `loop()`].  You can add some timing to test only so often:

```c++

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
```


### Step 4: IO

There is a chicken-and-egg problem with I/O.  If you have input and output errors, how can you tell there are problems?  You can hijack the return value to flip the idiot light on fail:

```c++
void packetReceive() {
    assertEqual(packetMessageCrc(), packetComputeCrc(),"io",idiotLightFlip());
    packetProcess();
}
```

If `packetRecieve` returns somethiing (not just `void`), there's a sneaky notation you can use:

   `(a,b,c)` evaluates `a`, then `b`, then is the value of `c`

So:

```c++
int packetReceive() {
    assertEqual(packetMessageCrc(), packetComputeCrc(),"io",(idiotLightFlip(),-1));
    return packetProcess();
}
```

flips the idiot light and returns -1 for `packetRecieve` if the checksum fails.

By passing a stream reference to IO operations, you can test if specific input and output steps work using the MockStream to simulate a serial port.  It is nice to set the default to the actual destination, so you don't have to type it everywhere:

```c++
int inInt(const char *name, Stream &io=Serial) {
    io.print("integer ");
    io.print(name);
    io.print("? ");
    return io.parseInt();
}

test(inInt) {
    MockStream ms;
    int xsend = 132;
    ms.input.println(xsend);
    int xrecv = inInt("x",ms);
    assertEqual(xsend,xrecv);
    assertEqual(ms.output,"integer x? ");
}

void useInInt() {
    int a = inInt("a");
    int b = inInt("b");
    /* ... */
}

```

Be careful!  MockStream uses dynamic memory to hold the input and output (they have the features of `Print` and `String` if you want to look them up).  Your device probably has very little memory, so:

* Don't create a lot of input or output in your test(s).  Each test should create and check
  relatively small (compared to the amount of free static RAM you have) amounts of data.
* Declare MockStreams inside tests/functions (not globals) so they can clean themselves up.

### Step 5: Time

It is important to know if something takes too long.  The following accounts for your system running long enough to suffer from the 32-bit microsecond counter rolling back through zero.

```c++
void timeCritical() {
    const int32_t maxMicros = 10000;
    uint32_t start = micros();
    /* ... */
    assertLess(int32_t(micros()-start),maxMicros);
}
```

### Step 6: Space

Recursion (functions that directly or indirectly use themselves to solve a reduced version of a problem) and dynamic memory (malloc or new operator, or dynamic structures like String that use malloc/new) create systems that just run out of room.

```c++
void spaceCritical() {
    const int16_t minBytes = 350;
    assertMore(freeMemory(), minBytes);
    /* ... */
}
```

Note that there is no guarantee the space can be allocated in one block.  The free space list may be fragmented, so you should check the outcome of any dynamic allocation to attempt.  Running out of memory is usually a critical failure, so add to your sanity checks:

```c++
bool criticalError = false;

test(sanity) {
  assertFalse(criticalError);
  /* ... */
}

void critical() {
     criticalError=true;
}

void dynamic(int bytes) {
   uint8_t *buf = (uint8_t*) malloc(bytes);
   assertNotEqual(buf,0,"memory",critical());
   /* ... */
}
```

## Meditations on The Heisenberg Uncertainty Principle

In physics, the Heisenberg Uncertainty Principle makes a specific statement about how well you can know where something is at (position) and simultaneously how fast it is moving (velocity). For small particles, this has to do with the impossibility of discovering where something is at without throwing something at it, which of course makes it move.

Stepping away from the finding bowling pins with bowling balls problem, there is a little HUP in many things.

Measuring something often changes it, and tests are a kind of observation of software. Now it is true that writing tests can change how an embedded system runs, and you may have to be careful when you spend time and resources testing so there is enough left to solve your problem.

But.

More importantly, writing tests changes how you write code.  Tests __observe__ code, and good tests are so valuable that you will want to adjust your habits so your code is easier to observe (hopefully operating correctly) with tests.

### Serve, don't rule

Code that insists on doing things one way is hard to test.  For example:

```c++
double F;
const int tempPin = 0;
void readTempF() {
    double K = analogRead(tempPin)*1024;
    double C = K-273;
    F=(9/5)*C-32;
}
```

`readTempF` is very insistent; marching from the activation of the analog-to-digital converter, to a specific sequence of transformations resulting in what we hope is the Fahrenheit temperature. It is incorrect (most code is initially), but there is little to do discover what part(s) are wrong.

Consider however:

```c++
double tempRaw() { // rule
    return analogRead(tempPin);
}

double tempRaw2K(double raw) { // serve
    return raw*1024;
}

double tempK2C(double K) { // serve
    return K - 273;
}

double tempC2F(double C) { // serve
    return (9/5)*C-32;
}

double tempRaw2F(double raw) { // serve
    double K = tempRaw2K(raw);
    double C = tempK2C(K);
    return tempC2F(C);
}

float F;

void readTempF() { // rule
    float raw = TempRaw();
    F = tempRaw2F(raw);
}

const int minTempRaw = 100;
const int maxTempRaw = 900;

testing(tempRaw) {
    double raw = tempRaw();
    assertLessOrEqual(minTempRaw,raw);
    assertLessOrEqual(raw, maxTempRaw);
}

test(tempRaw2K) {
    const float tolerance = 0.01;
    assertNear(tempRaw2K(minTempRaw),0.0,tolerance);
    assertNear(tempRaw2K(maxTempRaw),1000.0,tolerance);
}

test(tempK2C) {
    const float tolerance = 0.01;
    const float absZeroC = -273.15;
    assertNear(tempK2C(0.0),absZeroC,tolerance);
}

test(tempC2F) {
    const float tolerance = 0.01;
    const float freezeF = 32.0;
    const float boilF = 212.0;
    assertNear(tempC2F(0.0),freezeF,tolerance);
    assertNear(tempC2F(100.0),boilF,tolerance);
}
```

Ok, so the first and last parts `tempRaw` and  `readTempF` are still "rule", since they need to access specific things (the analog sensor and the global temperature F).  But all the parts are separated and the parts that can serve many purposes can be well tested.  Once the components are tested, it is easier to rely on them for the overall solution.  A good compiler will turn them both into the same code as well.

### Slow and Steady Wins the Race

Solve problems in little parts you can test. Then test the parts put together. Eventually you have a working hot dog cart.

Eventually you will be turning your hot dog cart into a taco stand.  Trust me everybody does. So leave in all the little parts and the little tests. This makes changing things much easier.

### A Journey of 1000 Miles Begins With A Map

Planning every detail ahead of time is a waste of effort. So is just charging into the woods. In software, you build the map with tests. If you don't know how to make a thing work, try writing the tests that should pass when it does work. The tests will help solidify what the Thing really is, including a lot about what it does not have to be.

### Your Momma Writes Better Tests Than You

You cannot see past your own design. Have other people suggest what needs to be tested. Have other people test it. Take their advice; your mother is usually right.


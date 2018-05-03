# En Vitro (Advanced)

ArduinoUnit can be used to compile sketches "en vitro" on the development system.

## Who

You need to be comfortable with compiling using the g++ compiler from the command line, and modifying bash scripts.

## Why

If a unit can tested outside of the embedded environment, you can find problems more easily because of the tools available.

## How

1. Write a sketch with tests (like `vitro.ino`)
1. Write a `mockVitro.cpp` (any name(s) will do) that creates a "mock" Arduino environment where the sketch can run on the dev system. ArduinoUnit provides limited mocking (just enough for ArduinoUnit to work):

    1. Stream -- CppStream(in=cin,out=cout) can be used to make a mock Serial device.
    1. Print
    1. String
    1. F() and __FlashStringHelper
    1. micros and millis
    1. You will probably want to mock more things.

1. `mockVitro.cpp` is a getting started version:

    1. A mock `Serial` connected to  stdin and stdout
    1. A `main()` that calls `setup()` then `loop()`s while tests are not done.

1. Compile everything using g++ (`./compile`)
1. Run the program locally (`./vitro`).

## Continuous Integration

There are a lot of continuous testing frameworks.  There is an `au2ju` python 2 script that converts the ArduinoUnit-style into JUnit style, which is a popular (although unreadable by humans) format for these tools:

```bash
./vitro | ./au2ju > vitro.xml
```

The `test` script runs the compiled executable and creates such a file.  Compile returns with an exit code of 1 if the build fails, and test returns with an exit code of 1 if the executable is missing or any test fails.

```bash
./compile
./test # creates vitro.log & vitro.xml
```

# ArduinoUnit Change Log

## 2.0.0

	* Rewrite

## 2.1

	* Updated documentation
	* Tests for FakeStreamBuffer
	* Support for Arduino Due
	* Support for Flash Strings (avoid namespace collision with Arduino String)
	* Reset of FakeStream

## 2.1.1

	* Support for Arduino Due (adjust examples)
	* Add library.properties file

## 2.2.0

	* Move to Arduino 1.5 compliant library directory structure
	* Move utility to ArduinoUnitUtility to avoid conflict with other libraries
	* Adjust firmware test to explain basic usage
	* Support for comparing various string types in (#iss47)
	* Add Compare.h and mkCompare in meta for comparing strings

## 2.3.0

	* Remove support of very old gcc compiler
	* Add support for de-duplicaton of flash strings
	* Add support for ESP 8266 and ESP 32
	* Use more standard file names (README.md, LICENSE, and CHANGELOG.md)
	* Use less likely to fail version of `while (!Serial) {}` in examples and documentation.
	* Make Test::TestString ArduinoUnitString, and refactor Compare to leverage that code.
	* Add an optional lazy message last argument to every assert.
	* Add building outside of arduino environment
	* Significant reduction of code implementation and compiled size (Compare.h is now small)
	* Add minimal mock builds on dev (in vitro) platform
	* Add better mock builds.
	* Use type widening to reduce specializations.
	* Depricate FakeStream for MockStream and MockPrint
	* Resolve a<b equivalence regresion (issue 77)
	* Resolve builds for ESP8266/ESP32
	* Prefer `decltype` over `__typeof__`
	* Added a guidebook
	* assertNear(a,b,max[,message]) for float/double
	* assertRelation(args [,message [,retval]])
	* assertRelativelyNear(a,b,max[,message]) for float/double
	* Test::abort() to complete all unresolved tests
	* Provide "en vitro" simple example & build scripts
## 3.0.0
	* Provide preliminary Continuous Integration Testing support (ci)
## 3.0.2
	* Data deduplication bug fix patch.
## 3.0.3
        * Support for Teensy (#87)
## 3.0.4
	* Skipped message bug fixed (#86)
	* Debug (alpha) documented for simavr/avr-gdb (ci.md)
	

ArduinoUnit 2.0 Firmware
========================

Firmware for testing ArduinoUnit 2.0 using ArduinoUnit 2.0.  This firmware
is of interest to developers of ArduinoUnit, not developers using ArduinoUnit.

The firmware is interactive --- you must choose a test configuration before
running some tests.  You can execute the default configuration by typing
'run' in the serial port monitor (9600 baud, send Newlines), but you can
interactively select and remove tests, and adjust overall verbosity before
running the active tests.

A family of tests can automatically be run with this firmware.
After defining your configuration in ./firmware/arscons.json, you can run
the tests with:

1. ./bin/go compile
1. ./bin/go upload
1. ./bin/go tests


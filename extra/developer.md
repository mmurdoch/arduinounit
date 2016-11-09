ArduinoUnit -- Developer Guide
==============================

How to build, test, and extend ArduinoUnit 2.0.
(see the readme on how to *use* ArduinoUnit 2.0).

###Version 2.0

##Layout

Here are the basic files and folders

* `readme.md` --- general user's guide
* `porting-guide.md` --- porting from 1.0 to 2.0
* `developer.md` --- starting guide for someone extending ArduinoUnit.
* `src` --- content of ArduinoUnit that should be copied to the user or
          system-wide arduino libraries folder.
* `firmware` --- the firmware to test the functionality of the
          the library (tested on mega2560).
* `tests` --- various tests, the output from these tests, and the result
            of comparing these against known results.
* `bin` --- helper files & scripts.

##Setup

* Install python 2.7
  
  - Windows: try ActiveState python (reboot).
  - OS X: already installed.
  - Linux: already installed.

  Typing `python -v` at the command prompt should give version information
  about python.

* Install Arduino IDE

  - Windows: If you have version 1.03, I suggest you copy the Zip file to
    C:\Arduino103

  - OSX Drag App to /Applications folder

  - Linux sudo apt-get install arduino, or sudo yum install arduino, or
    check your distribution advice.

  Make sure you can start the IDE and download Blink to your dev board,
  and note which serial port you use to communicate with the board.

* Install Git

  - Windows: Install the git-bash tool.

  - OS X Install XCode and choose to install command line tools.

  - Linux `sudo apt-get install git` or `sudo yum install git-core`

  From here on, I will assume you are working from the git-bash
  shell in Windows, or a bash command prompt.

* Install pyserial 2.6 (https://pypi.python.org/pypi/pyserial)

  Download tar.gz file and type:
  <pre>
      tar zxf pyserial-2.6.tar.gz
      cd pyserial-2.6
  </pre>
  
  Then, if on OS X or Linux, type:
  <pre>
      sudo python setup.py install
  </pre>
  
  or, if on Windows, type:
  <pre>
      python setup.py install
  </pre>

* Install Scons (scons.org).

  The MSI failed for me in windows.  Get the tar file and install as 
  pyserial above.

* Copy `firmware/arscons.json.{win,osx,linux}` to `firmware/arscons.json`
  and edit for local settings

  A leading tile (~) in ARDUINO_HOME, AVR_HOME, and SKETCHBOOK_HOME is
  replaced by the user's home directory.

* Build, upload, monitor, run and test from the project root folder

  <pre>
    ./bin/go compile
    ./bin/go upload
    ./bin/go monitor
    ./bin/go run < tests/<some-test>.in
    ./bin/go test < tests/<some-test>.in
    ./bin/go tests
  </pre>

* Run the testing firmware

  The firmware is interactive, so you can change test configurations
  before running the test suite.  You can run the arduino serial monitor
  at 9600 baud and line endings set to either 'Newline' or 'Both NL & CR'
  to see the output.  For example:
  <pre>
      command> run
      ...
      test overall passed.
      test summary: 6 passed, 1 failed, and 1 skipped, out of 8 test(s).
  </pre>
  or
  <pre>
      command> exclude *
      command> include assert*
      command> min_verbosity 00111111
      command> run
      ...
  </pre>
  You can use the prompted commands to change the test set and the verbosity
  of the tests.

##License

Copyright (c) 2013 Warren MacEvoy, Matthew Murdoch

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

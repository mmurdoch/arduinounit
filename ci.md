# CI - Continuous Integration

This is preliminary support for continuous integration.  These have been tested in Linux (Ubuntu 16.04) and Mac OSX.  These are tool agnostic: you can run them by hand, as part of a commit hook (no server), or using a continuous integration service like Travis CI or Jenkins.  They also support en vitro, simulator, and en vivo tests.

## Install

There really is nothing to install.  The CI tools are bash scripts that can be run from anywhere.  They maintain a set of IDE's under the $HOME/.arduino directory so you can test your project with different versions of the IDE and different installed libraries.

### Prerequisites

#### macosx

On a OS X, you will have to install the XCode command line tools and homebrew:

```bash
if ! which git ; then xcode-select --install ; fi # xcode cli
if ! which brew ; then /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)" ; fi # homebrew
```

#### linux

In linux, you need git make g++ unzip and curl:

```bash
sudo apt install -y git make g++ unzip curl
```

#### windows

Install Windows Subsystem for Linux, then Ubuntu under WSL, then follow the linux instructions in a WSL/Ubuntu terminal.  I think.

This has not been tested in windows.  I believe using the Windows Subsystem for Linux and installing Ubuntu 16.04 would be a path to success [WSL](https://devtidbits.com/2017/06/08/run-ubuntu-16-04-on-windows-10-creators-update/), although some tweaks may be necessary.  __All commands are assumed to be running from a bash shell.___

### Setup

Copy the arduinounit (or at least the ci sub-folder) and its contents to clone this repository in your projects folder, as in:

```bash
# the directory is where you keep your projects...
mkdir -p "$HOME/projects"
cd "$HOME/projects"
git clone https://github.com/mmurdoch/arduinounit.git
```

To make the tools easier to use, you can define the `au` bash function by sourcing `au` in the `ci` directory (notice the dot with a space):

```bash
. "$HOME/projects/arduinounit/ci/au"

```

You can put this in your `.profile` if you like, so it is set every time you log in.  

To test (you may have to log out and log back in), try:

```bash
au dir
au dir_cache
au arduino_dir
au sketchbook
au arduino --pref update.check=false --install-library ArduinoUnit
```

In the `examples/basic` arduinounit folder, try,

```bash
au compile_uno
au simavr_uno
```

If you want to see the output in junit format (you really don't, but Jenkins, TravisCI and similar CI tools do), try

```bash
au simavr_uno | au au2ju
```

In the vitro example, you can run tests 3 different ways: on the hardware, on the simulator, or on the dev system:

```bash
au compile_vitro
./vitro 
./vitro | au au2ju
au compile_uno
au simavr_uno --hex=vitro.hex
au upload_uno
```

You may have to specify --port=[port] to upload, assuming an UNO board is connected.

These all take `--os=(macosx|linux64|linux32|linuxarm|windows)` and `--version=(arduino_ide_version)` options.

### Important

The CI tools do NOT use the standard install location for arduino.  This is because tool configuration tests [checking if your project builds with certain version of the IDE or dependent librarie(s)] would be very inconvenient if that required the removal and installation of your standard arduino environment.  Instead the IDE and simavr tools are cached in a directory structure:

```bash
${HOME}/.arduino/
  cache/
    arduino-${VERSION}-${OS}(.zip|.tar.xz)
    arduino-${VERSION}-${OS}(.zip|.tar.xz).sha512sum.txt
    simavr/
      ... git clone https://github.com/buserror/simavr .
    ${VERSION}-${OS}/
      ...
  portable/
    sketchbook/
      libraries/
```

The CI tools install the IDE in "portable" mode so they do not use or modify your default IDE. When making a "clean install test", do NOT delete the zip/tar files in the cache.  These are checked against the very secure sha512 (and will be re-downloaded if they checksum fails).  Downloading these again is very slow and a waste of bandwidth.

A clean (except the ide cache which is pointless and slow) install can be started with:

```bash
au reset
```

## debug

*Warning* this is a hack.  But stepping through code is amazingly useful.  I have only worked out steps on Ubuntu 18.04, and depends on the simavr tool, which gives no uart output.  Still it is better than nothing.

1. Install simavr (sudo apt install simavr).  This should also install avr-gcc.
1. Install this toolset in in Ubunutu: run `au arduino` to spin up the basic working cache of the ide.
1. Create and set the TMP environment variable so the system does not build and erase a temporary file.  I used "<project>/tmpdir" and made that folder.
1. Compile the project with `au compile_avr` in the project directory.  The build residue should be in TMP.
1. Look in the TMP directory, and find the build folder.
1. Open two terminals here; one to run the program and one to debug from.
1. In your run terminal, type
```bash
simavr -g -m atmega328p -f 16000000 basic.ino.elf
```
1. In your debug terminal, type `avr-gdb` to start the avr debugger, then in th prompts type
```bash
tui enable
file basic.ino.elf
target remote :1234
b setup
c
s
```

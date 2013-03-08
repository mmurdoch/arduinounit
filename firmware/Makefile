#
# Useful targets:
#
#   make             - no upload
#   make upload      - upload to target
#   make clean       - remove all our dependencies
#   make depends     - update dependencies
#   make reset       - reset the Arduino by tickling DTR on the serial port
#   make raw_upload  - upload without first resetting
#   make show_boards - list all the boards defined in boards.txt
#   make monitor     - connect to the Arduino's serial port
#
# Uses Arduino-Makefile, from 
#
#   https://github.com/mjoldfield/Arduino-Makefile.git
#
# See the ../Arduino-Makefile/README.md for further information, online at
#
#   https://github.com/mjoldfield/Arduino-Makefile
#

UNAME := $(shell uname)

ARDUINO_SKETCHBOOK=$(HOME)/Documents/Arduino

ifeq ($(UNAME), Linux)
# guess - not tested
  ARDUINO_DIR=/usr/share/arduino
  ARDUINO_PORT=/dev/ttyACM*
  AVR_TOOLS_PATH=/usr/bin
  AVRDUDE_CONF=/etc/avrdude/avrdude.conf
  DOXYGEN=doxygen
endif

ifeq ($(UNAME), Darwin)
  ARDUINO_DIR=/Applications/Arduino.app/Contents/Resources/Java
  ARDUINO_PORT=/dev/cu.usb*
  DOXYGEN=doxygen
endif

# BOARD_TAG=mega2560
BOARD_TAG=uno
ARDMK_DIR=..

MONITOR_BAUDRATE=$(shell cd ..; ./bin/baud)

ARDUINO_LIBS=ArduinoUnit ArduinoUnit/utility

all : firmware.ino build-$(BOARD_TAG)/firmware.hex

include Arduino.mk

doc : 
	cd ../src; $(DOXYGEN)

doc-clean : 
	/bin/rm -rf ../doc/doxygen

install : 
	mkdir -p $(ARDUINO_DIR)/libraries/ArduinoUnit
	/bin/rm -rf "$(ARDUINO_DIR)/libraries/ArduinoUnit/"*
	rm -rf ../src/*~ ../src/*/*~ ../src/#* ../src/.#* ../src/utility/.#*
	cp -r ../src/* $(ARDUINO_DIR)/libraries/ArduinoUnit

port :
	@cd ..; ./bin/port

baud :
	@echo $(MONITOR_BAUDRATE)

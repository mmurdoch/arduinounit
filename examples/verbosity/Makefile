ARDUINO_UNIT_SRC_DIR=../../src
ARDUINO_UNIT_SRC=${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnitString.cpp ${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnit.cpp

verbosity : main.cpp verbosity.ino ${ARDUINO_UNIT_SRC}
	g++ -isystem ../../src -std=gnu++11 -o verbosity main.cpp ${ARDUINO_UNIT_SRC}


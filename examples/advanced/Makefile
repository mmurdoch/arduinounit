ARDUINO_UNIT_SRC_DIR=../../src
ARDUINO_UNIT_SRC=${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnitString.cpp ${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnit.cpp

advanced : main.cpp advanced.ino ${ARDUINO_UNIT_SRC}
	g++ -isystem ../../src -std=gnu++11 -o advanced main.cpp ${ARDUINO_UNIT_SRC}


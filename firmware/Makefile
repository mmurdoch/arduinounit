ARDUINO_UNIT_SRC_DIR=../src
ARDUINO_UNIT_SRC=${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnitString.cpp ${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnit.cpp

firmware : main.cpp firmware.ino ${ARDUINO_UNIT_SRC}
	g++ -isystem ${ARDUINO_UNIT_SRC_DIR} -std=gnu++11 -o firmware main.cpp ${ARDUINO_UNIT_SRC}

ARDUINO_UNIT_SRC_DIR=../src
ARDUINO_UNIT_SRC=${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnitString.cpp ${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnit.cpp

ARDUINO_UNIT_MOCK=${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnitMockWString.cpp

mockString : mockString.cpp ${ARDUINO_UNIT_SRC} ${ARDUINO_UNIT_MOCK}
	g++ -isystem ${ARDUINO_UNIT_SRC_DIR} -std=gnu++11 -o $@ mockString.cpp ${ARDUINO_UNIT_SRC} ${ARDUINO_UNIT_MOCK}

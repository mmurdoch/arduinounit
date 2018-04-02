ARDUINO_UNIT_SRC_DIR=../src
ARDUINO_UNIT_SRC=${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnitString.cpp ${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnit.cpp

ARDUINO_UNIT_MOCK=${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnitMockWString.cpp ${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnitMockPrint.cpp ${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnitMockPrintable.cpp ${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnitMockTime.cpp  ${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnitMockStream.cpp

mockTime : mockTime.cpp ${ARDUINO_UNIT_SRC} ${ARDUINO_UNIT_MOCK}
	g++ -g -isystem ${ARDUINO_UNIT_SRC_DIR} -std=gnu++11 -o $@ $< ${ARDUINO_UNIT_SRC} ${ARDUINO_UNIT_MOCK}

mockString : mockString.cpp ${ARDUINO_UNIT_SRC} ${ARDUINO_UNIT_MOCK}
	g++ -g -isystem ${ARDUINO_UNIT_SRC_DIR} -std=gnu++11 -o $@ $< ${ARDUINO_UNIT_SRC} ${ARDUINO_UNIT_MOCK}

mockPrint : mockPrint.cpp ${ARDUINO_UNIT_SRC} ${ARDUINO_UNIT_MOCK}
	g++ -g -isystem ${ARDUINO_UNIT_SRC_DIR} -std=gnu++11 -o $@ $< ${ARDUINO_UNIT_SRC} ${ARDUINO_UNIT_MOCK}

mockStream : mockStream.cpp ${ARDUINO_UNIT_SRC} ${ARDUINO_UNIT_MOCK}
	g++ -g -isystem ${ARDUINO_UNIT_SRC_DIR} -std=gnu++11 -o $@ $< ${ARDUINO_UNIT_SRC} ${ARDUINO_UNIT_MOCK}

java -cp /Applications/Arduino.app/Contents/Java/* \
  -DAPP_DIR=/Applications/Arduino.app/Contents/Java \
  -Dfile.encoding=UTF-8 -Dapple.awt.UIElement=true \
  -Xms128M -Xmx512M processing.app.Base \
  --verify /path/to/sketch/sketch.ino

ARDUINO_UNIT_SRC_DIR=../src
ARDUINO_UNIT_SRC=${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnitString.cpp ${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnit.cpp

ARDUINO_UNIT_MOCK=${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnitMockTime.cpp ${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnitMockWString.cpp ${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnitMockPrint.cpp ${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnitMockPrintable.cpp ${ARDUINO_UNIT_SRC_DIR}/ArduinoUnitUtility/ArduinoUnitMockStream.cpp

mockFirmware : mockFirmware.cpp firmware.ino ${ARDUINO_UNIT_SRC}
	g++ -isystem ${ARDUINO_UNIT_SRC_DIR} -std=gnu++11 -o $@ $< ${ARDUINO_UNIT_SRC} ${ARDUINO_UNIT_MOCK}

install :
	$(MAKE) -C .. install

firmware-mega : install mockFirmware.cpp firmware.ino ${ARDUINO_UNIT_SRC}
	rm .#*
	./run-arduino --verify firmware.ino

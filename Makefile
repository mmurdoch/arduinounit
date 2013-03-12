ifeq ($(UNAME), Linux)
  ARDUINO_DIR=/usr/share/arduino
endif
ifeq ($(UNAME), Darwin)
  ARDUINO_DIR=/Applications/Arduino.app/Contents/Resources/Java
endif

DOXYGEN=doxygen

all : firmware doc
	$(MAKE) -C firmware all

.PHONY: doc
doc :
	$(DOXYGEN)

.PHONY: test
test :
	./bin/go compile upload test

.PHONY: firmware
firmware:
	./bin/go compile

.PHONY: install
install : 
	mkdir -p $(ARDUINO_DIR)/libraries/ArduinoUnit
	/bin/rm -rf "$(ARDUINO_DIR)/libraries/ArduinoUnit/"*
	rm -rf src/*~ src/*/*~ src/\#* src/.\#* src/utility/.\#*
	cp -r src/* $(ARDUINO_DIR)/libraries/ArduinoUnit

.PHONY: clean
clean :
	/bin/rm -rf `find . -name '*~' -o -name '#*' -o -name '.#*'`

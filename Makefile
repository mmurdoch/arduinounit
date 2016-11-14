UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
  ARDUINO_DIR=/usr/share/arduino
endif
ifeq ($(UNAME), Darwin)
  ARDUINO_DIR=$(HOME)/Documents/Arduino
endif

DOXYGEN=doxygen

all : firmware doc

.PHONY: doc
doc :
	$(DOXYGEN)

.PHONY: test
test :
	./bin/go compile upload tests

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

.PHONY: scrapetests
scrapetests:
	for f in tests/*.in ; do echo "$$f..."; ./bin/go run < $$f > $${f%%.in}.out; done

.PHONY: updatetests
updatetests:
	for f in tests/*.in ; do cp $${f%%.in}.out $$f; done

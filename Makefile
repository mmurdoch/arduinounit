UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
  ARDUINO_DIR=$(HOME)/Arduino
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
	/bin/rm -rf "${ARDUINO_DIR}/libraries/ArduinoUnit"
	mkdir -p ${ARDUINO_DIR}/libraries/ArduinoUnit
	find . -path './.*' -prune -false -o -type f -a -regex '.*/[a-zA-Z0-9][-_a-zA-Z0-9.]*[a-zA-Z0-9]' > install-files
	tar -cf - -T install-files | tar -C "${ARDUINO_DIR}/libraries/ArduinoUnit" -xvf -

.PHONY: clean
clean :
	/bin/rm -rf `find . -name '*~' -o -name '#*' -o -name '.#*'`

.PHONY: scrapetests
scrapetests:
	for f in tests/*.in ; do echo "$$f..."; ./bin/go run < $$f > $${f%%.in}.out; done

.PHONY: updatetests
updatetests:
	for f in tests/*.in ; do cp $${f%%.in}.out $$f; done

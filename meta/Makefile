all : ../src/ArduinoUnitUtility/Compare.h
mkCompare : mkCompare.cpp
	$(CXX) -o mkCompare mkCompare.cpp
Compare.h: mkCompare
	./mkCompare --flash --string
../src/ArduinoUnitUtility/Compare.h : Compare.h
	cp Compare.h ../src/ArduinoUnitUtility/Compare.h
testWiden : testWiden.cpp
	$(CXX) -o testWiden testWiden.cpp

all : ../src/ArduinoUnitUtility/Compare.h
mkCompare : mkCompare.cpp
	$(CXX) -std=c++11 -o mkCompare mkCompare.cpp
Compare.h: mkCompare
	./mkCompare --flash --string
../src/ArduinoUnitUtility/Compare.h : Compare.h
	cp Compare.h ../src/ArduinoUnitUtility/Compare.h

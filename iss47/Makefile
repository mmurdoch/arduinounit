mkCompare : mkCompare.cpp
	$(CXX) -o mkCompare mkCompare.cpp
Compare.h: mkCompare
	./mkCompare --flash --string
../src/Compare.h : Compare.h
	cp Compare.h ../src/Compare.h

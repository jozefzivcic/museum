# Jozef Zivcic PV112 project

binary = museum
CXX = g++
CXXFLAGS := -std=c++11 -lGL -lglut -lGLEW -lIL $(CXXFLAGS)

result: $(binary)

$(binary): museum.cpp PV112.h
	$(CXX) -o $@ $< PV112.cpp $(CXXFLAGS)

clean:
	rm $(binary)

CXX = g++
CXXFLAGS = -std=c++11
LIBS = -lyaml-cpp

SOURCES = main.cpp
EXECUTABLE = a.out

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	rm -f $(EXECUTABLE)

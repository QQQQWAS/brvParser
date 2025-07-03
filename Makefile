appname := main

CXX := g++
CXXFLAGS := -Wall -g -Iinclude -lglfw -lGL
srcfiles := $(shell find . -maxdepth 1 -name "*.cpp") $(shell find ./include -maxdepth 2 -name "*.cpp") $(shell find ./include -maxdepth 2 -name "*.c")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: $(appname)
	@./$(appname)

$(appname): $(objects)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(appname) $(objects) $(LDLIBS)

depend: .depend

.depend: $(srcfiles)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	rm -f $(objects)

dist-clean: clean
	rm -f *~ .depend

include .depend

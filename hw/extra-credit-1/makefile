CXX = g++
CXXFLAGS = -Wall -pedantic -O2 -g

all: malware

malware: example.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f *.o malware

.PHONY: clean all
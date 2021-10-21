CXX = clang++

CXXFLAGS = -Wall -Wextra -pedantic -c

RM = rm -f

demo: main.o
	$(CXX) $? -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) $? -o $@

clean:
	$(RM) *.o demo

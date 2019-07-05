CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2
EXE = tema1

AUTHOR = VanceaCosminRazvan
GROUP = 313CA
HOMEWORK = 1

build: Source.o
	$(CXX) $(CXXFLAGS) -o $(EXE) $^

Source.o: src/Source.cpp
	$(CXX) $(CXXFLAGS) -c $^	


pack: Makefile README.md
	zip -q -9 "$(GROUP)_$(AUTHOR)_Tema$(HOMEWORK).zip" src/ $^

clean_objects:
	rm -f *.o
clean: clean_objects
	rm -f $(EXE)
	rm -f races.out

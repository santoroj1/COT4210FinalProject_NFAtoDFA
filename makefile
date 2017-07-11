build:	*.h *.cpp
	g++ -std=c++0x -fpermissive *.h *.cpp
run:	a.out
	./a.out

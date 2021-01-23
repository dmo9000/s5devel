CXX = g++

%.o : %.cpp
	g++ -DNPKG -c $< -o $@


npkg: npkg.o
	g++ -DNPKG -o npkg npkg.o




clean:
	rm -f npkg *.o


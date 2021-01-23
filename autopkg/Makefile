CXX = g++

%.o : %.cpp
	g++ -DNPKG -c $< -o $@


autopkg: autopkg.o
	g++ -DNPKG -o autopkg autopkg.o




clean:
	rm -f autopkg *.o


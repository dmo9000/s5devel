CXX = g++

%.o : %.cpp
	g++ -D__NDT__ -c $< -o $@


ndt: ndt.o
	g++ -D__NDT__ -o ndt ndt.o




clean:
	rm -f ndt *.o


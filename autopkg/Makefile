CXX = g++
CXXLDFLAGS=-L/usr/lib64 -L/lib64 -lssl -lidn2 -lcrypto -lbrotlidec -lz -lpthread -lc -ldl -lbrotlicommon -lm -lcurl 
AUTOPKG_OBJS=autopkg.o groupinstall.o

%.o : %.cpp
	g++ -DNPKG -c $< -o $@


autopkg: $(AUTOPKG_OBJS)
	g++ -DNPKG $(CXXLDFLAGS) -o autopkg $(AUTOPKG_OBJS)




clean:
	rm -f autopkg *.o


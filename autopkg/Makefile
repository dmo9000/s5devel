CXX = g++
CXXFLAGS = -std=c++20
CXXLDFLAGS=-L/usr/lib64 -L/lib64 -lssl -lidn2 -lcrypto -lbrotlidec -lz -lpthread -lc -ldl -lbrotlicommon -lm -lcurl 
AUTOPKG_OBJS=autopkg.o groupinstall.o pkginstall.o httpdriver.o subprocess.o

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -DNPKG -c $< -o $@


autopkg: $(AUTOPKG_OBJS)
	$(CXX) $(CXXFLAGS) -DNPKG $(CXXLDFLAGS) -o autopkg $(AUTOPKG_OBJS)

clean:
	rm -f autopkg *.o


CXX = /home/yxc/software/openmpi/bin/mpicxx
source = $(wildcard *.cpp)
object = $(patsubst %.cpp, %.o, $(source))
CXXFLAGS = -I/home/yxc/software/dealii/include -I/home/yxc/software/dealii/include/deal.II -I/home/yxc/software/boost/include  -std=c++11 -D__SERIALIZATION__ -DMULTITHREAD -pthread -fPIC -ftemplate-depth-256 -I/home/yxc/software/AFEPack-master/library/include
LIBS = -L/home/yxc/lib -ldeal_II -lAFEPack -lm -ldl
all : main

%.o : %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

main : $(object)
	$(CXX) -o $@ $(object) $(LIBS)

clean :
	-rm -rf $(object)
	-rm -rf main

.PHONY : default clean
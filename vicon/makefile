ARCH_TEMP = $(shell getconf LONG_BIT)
ARCH_32 = 32
ARCH_64 = 64
ARCH_   = 32
ARCH = $(ARCH_$(ARCH_TEMP))

LIB_DIR = ../lib

CXX = g++
OBJ = ViconClass.o
CPPFLAGS = -Wall -std=c++11
LIBS = -L$(LIB_DIR) -lViconDataStreamSDK_CPP_$(ARCH)
DEPS = ViconClass.cpp

example: $(OBJ)
	$(CXX) $(CPPFLAGS) exampleViconClass.cpp $(OBJ) $(LIBS) -o exampleViconClass.exe

#ViconClass.o:
#	$(CXX) $(CPPFLAGS) ViconClass.cpp $(LIBS) -o ViconClass.o 

clean: 
	rm -rf *.o *~ *.exe

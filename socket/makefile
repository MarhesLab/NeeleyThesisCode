# Compiler
CXX = g++
CPPFLAGS = -Wall -std=c++11

# Directories for files
DIR_F := ../functions
DIR_TOP := ..
DIR_S := .

# Object File Declarations
OBJ_CLI = $(DIR_S)/QuadClient.o
OBJ_DEF = $(DIR_TOP)/defs.o
OBJ_PAC = $(DIR_S)/packet.o $(OBJ_DEF)
OBJ_SER = $(DIR_S)/QuadServer.o
OBJ_ALL = $(OBJ_CLI) $(OBJ_SER)

clientTest: $(OBJ_CLI)
	$(CXX) $(CPPFLAGS) socketClientTest.cpp $(OBJ_CLI) -o clientTest.exe

serverTest: $(OBJ_SER)
	$(CXX) $(CPPFLAGS) socketServerTest.cpp $(OBJ_SER) -o serverTest.exe

clientPacket: $(OBJ_CLI) $(OBJ_PAC)
	$(CXX) $(CPPFLAGS) packetClientTest.cpp $(OBJ_CLI) $(OBJ_PAC) -o clientPacket.exe

serverPacket: $(OBJ_SER) $(OBJ_PAC)
	$(CXX) $(CPPFLAGS) packetServerTest.cpp $(OBJ_SER) $(OBJ_PAC) -o serverPacket.exe

clean:
	rm -rf *.o *.exe *~

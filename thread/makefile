# Compiler
CXX = g++
CPPFLAGS = -Wall -std=c++11

# Architecture Flags
# (the weird format works around the Intel Edison's lack of a "getconf" command)
ARCH_TEMP = $(shell getconf LONG_BIT)
ARCH_32 = 32
ARCH_64 = 64
ARCH_   = 32
ARCH = $(ARCH_$(ARCH_TEMP))

# Directories for files
DIR_F := ../functions
DIR_L := ../lib
DIR_Q := ../quad
DIR_S := ../socket
DIR_T := .
DIR_TOP := ..
DIR_V := ../vicon

# Object File Declarations
OBJ_ATH = $(DIR_T)/AutopilotThread.o $(OBJ_AUT) $(OBJ_DEF) $(OBJ_TIM)
OBJ_GTH = $(DIR_T)/GcsServerThread.o $(OBJ_QSE) $(OBJ_PAC) $(OBJ_DEF)
OBJ_LTH = $(DIR_T)/LoggerThread.o 
OBJ_QTH = $(DIR_T)/QuadClientThread.o $(OBJ_QCL) $(OBJ_PAC) $(OBJ_DEF)
OBJ_VTH = $(DIR_T)/ViconThread.o $(OBJ_VIC) $(OBJ_TIM) 

OBJ_AUT = $(DIR_F)/autopilot.o
OBJ_DEF = $(DIR_TOP)/defs.o
OBJ_PAC = $(DIR_S)/packet.o
OBJ_QCL = $(DIR_S)/QuadClient.o
OBJ_QSE = $(DIR_S)/QuadServer.o
OBJ_TIM = $(DIR_F)/Timer.o
OBJ_VIC = $(DIR_V)/ViconClass.o

# Library declarations
LIB_THD = -lpthread
LIB_VIC = -L$(DIR_L) -lViconDataStreamSDK_CPP_$(ARCH)

exampleAutopilot: exampleAutopilotThread.o $(OBJ_ATH)
	$(CXX) $(CPPFLAGS) exampleAutopilotThread.o $(OBJ_ATH) $(LIB_THD) -o autopilotThreadExample.exe

exampleGcsServerThread: exampleGcsServerThread.o $(OBJ_GTH) $(OBJ_TIM)
	$(CXX) $(CPPFLAGS) exampleGcsServerThread.o $(OBJ_GTH) $(OBJ_TIM) $(LIB_THD) -o gcsServerThreadExample.exe

exampleLoggerThread: exampleLoggerThread.o $(OBJ_LTH)
	$(CXX) $(CPPFLAGS) exampleLoggerThread.o $(OBJ_LTH) $(LIB_THD) -o loggerThreadExample.exe

exampleQuadClientThread: exampleQuadClientThread.o $(OBJ_QTH) $(OBJ_TIM)
	$(CXX) $(CPPFLAGS) exampleQuadClientThread.o $(OBJ_QTH) $(OBJ_TIM) $(LIB_THD) -o quadClientThreadExample.exe

exampleVicon: exampleViconThread.o $(OBJ_VTH) $(OBJ_DEF)
	$(CXX) $(CPPFLAGS) exampleViconThread.o $(OBJ_VTH) $(OBJ_DEF) $(LIB_THD) $(LIB_VIC) -o viconThreadExample.exe

clean:
	rm -rf logfile_example.csv *.o *.exe $(DIR_F)/*.o $(DIR_V)/*.o $(DIR_S)/*.o *~

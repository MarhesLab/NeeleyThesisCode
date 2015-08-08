# Compiler
CXX = g++

# Architecture Flags
# (the weird format works around the Intel Edison's lack of a 
# "getconf" command in Yocto Linux; not a problem in Ubilinux)
ARCH_TEMP = $(shell getconf LONG_BIT)
ARCH_32 = 32
ARCH_64 = 64
ARCH_   = 32
ARCH = $(ARCH_$(ARCH_TEMP))
CPPFLAGS = -Wall -std=c++11


# Directories for files
DIR_F := ../functions
DIR_L := ../lib
DIR_M := ../matrix
DIR_O := ../opt
DIR_Q := .
DIR_S := ../socket
DIR_T := ../thread
DIR_TOP := ..
DIR_V := ../vicon

# Libraries
LIB_MAT = -L$(DIR_L) -lMatlabFunctions$(ARCH)
LIB_OCV = $(shell pkg-config opencv --libs --cflags)
LIB_OPT = -lglpk -lm
LIB_THD = -lpthread
LIB_VIC = -L$(DIR_L) -lViconDataStreamSDK_CPP_$(ARCH)
LIB_ALL = $(LIB_OPT) $(LIB_THD) $(LIB_VIC) $(LIB_OCV) 

# Object File Declarations
OBJ_QUA = $(DIR_Q)/Quadrotor.o $(OBJ_ATH) $(OBJ_LTH) $(OBJ_QTH) $(OBJ_VTH) $(OBJ_DEF) $(OBJ_FIL) $(OBJ_MOD) $(OBJ_PAR) $(OBJ_TRA)

OBJ_ATH = $(DIR_T)/AutopilotThread.o $(OBJ_AUT) $(OBJ_TIM)
OBJ_GTH = $(DIR_T)/GcsServerThread.o $(OBJ_PAC) $(OBJ_QSE)
OBJ_LTH = $(DIR_T)/LoggerThread.o 
OBJ_QTH = $(DIR_T)/QuadClientThread.o $(OBJ_PAC) $(OBJ_QCL)
OBJ_VTH = $(DIR_T)/ViconThread.o $(OBJ_VIC)

OBJ_AUT = $(DIR_F)/autopilot.o
OBJ_CON = $(DIR_F)/Controller.o $(OBJ_YAW) $(OBJ_FBL)
OBJ_DEF = $(DIR_TOP)/defs.o
OBJ_FBL = $(DIR_F)/feedbackLin.o
OBJ_FIL = $(DIR_F)/Filter3.o
OBJ_LOG = $(DIR_F)/Logger2.o
OBJ_MAT = $(DIR_M)/Matrix2.o
OBJ_MOD = $(DIR_Q)/modes.o $(OBJ_CON) $(OBJ_TRJ)
OBJ_PAC = $(DIR_S)/packet.o 
OBJ_PAR = $(DIR_Q)/parameters.o
OBJ_PRL = $(DIR_O)/PitchRollLimiter.o
OBJ_QCL = $(DIR_S)/QuadClient.o
OBJ_QSE = $(DIR_S)/QuadServer.o
OBJ_TIM = $(DIR_F)/Timer.o
OBJ_TRA = $(DIR_F)/transforms.o
OBJ_TRJ = $(DIR_F)/TrajectoryMaker.o
OBJ_VIC = $(DIR_V)/ViconClass.o
OBJ_YAW = $(DIR_O)/YawOptimizer.o

main: mainProgram.o $(OBJ_QUA)
	$(CXX) $(CPPFLAGS) mainProgram.o $(OBJ_QUA) $(LIB_ALL) -o mainProgram.exe

clean:
	rm -rf *.o *~ mainProgram.exe

cleanall: 
	rm -rf *.o *~ mainProgram.exe
	rm -rf $(DIR_F)/*.o $(DIR_F)/*~ 
	rm -rf $(DIR_M)/*.o $(DIR_M)/*~
	rm -rf $(DIR_O)/*.o $(DIR_O)/*~
	rm -rf $(DIR_S)/*.o $(DIR_S)/*~ 
	rm -rf $(DIR_T)/*.o $(DIR_T)/*~
	rm -rf $(DIR_TOP)/*.o $(DIR_TOP)/*~ 
	rm -rf $(DIR_V)/*.o $(DIR_V)/*~

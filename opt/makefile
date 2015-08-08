

# NOTE: MAKE SURE GLPK IS INSTALLED FIRST. READ "glpk.pdf" IN "./glpk-4.55/doc"

# Compiler and Architecture Flags
CXX = g++
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
DIR_O := .
DIR_Q := ../quad
DIR_S := ../socket
DIR_T := ../thread
DIR_TEST := ../test
DIR_TOP := ..
DIR_V := ../vicon


# Libraries
LIB_VIC = -L$(DIR_L) -lViconDataStreamSDK_CPP_$(ARCH)
LIB_MAT = -L$(DIR_L) -lMatlabFunctions$(ARCH)
LIB_THD = -lpthread
LIB_OPT = -lglpk -lm

# Object File Declarations
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



runminmaxyawtorque: runminmaxyawtorque.o $(OBJ_SPF)
	$(CXX) $(CPPFLAGS) runminmaxyawtorque.o $(OBJ_SPF) $(LIBS) -o runminmaxyawtorque.exe

runminmaxyawtorque.o: 
	$(CXX) $(CPPFLAGS) -c runminmaxyawtorque.cpp
	
simplex: $(OBJ_YAW) $(OBJ_LTH) $(OBJ_TIM) $(OBJ_DEF) $(OBJ_PAR)
	$(CXX) $(CPPFLAGS) SimplexFeasibilitySearch.cpp $(OBJ_YAW) $(OBJ_LTH) $(OBJ_TIM) $(OBJ_DEF) $(OBJ_PAR) $(LIB_OPT) $(LIB_THD) -o simplexSearch.exe

simplexPR: $(OBJ_PRL) $(OBJ_LTH) $(OBJ_TIM) $(OBJ_DEF) $(OBJ_PAR)
	$(CXX) $(CPPFLAGS) SimplexFeasibilitySearchPR.cpp $(OBJ_PRL) $(OBJ_LTH) $(OBJ_TIM) $(OBJ_DEF) $(OBJ_PAR) $(LIB_OPT) $(LIB_THD) -o simplexSearchPR.exe
	
interior: $(OBJ_YAW) $(OBJ_LTH) $(OBJ_TIM) $(OBJ_DEF) $(OBJ_PAR)
	$(CXX) $(CPPFLAGS) InteriorFeasibilitySearch.cpp $(OBJ_YAW) $(OBJ_LTH) $(OBJ_TIM) $(OBJ_DEF) $(OBJ_PAR) $(LIB_OPT) $(LIB_THD) -o interiorSearch.exe
	
interiorPR: $(OBJ_PRL) $(OBJ_LTH) $(OBJ_TIM) $(OBJ_DEF) $(OBJ_PAR)
	$(CXX) $(CPPFLAGS) InteriorFeasibilitySearchPR.cpp $(OBJ_PRL) $(OBJ_LTH) $(OBJ_TIM) $(OBJ_DEF) $(OBJ_PAR) $(LIB_OPT) $(LIB_THD) -o interiorSearchPR.exe

clean: 
	rm *.o *.exe *~

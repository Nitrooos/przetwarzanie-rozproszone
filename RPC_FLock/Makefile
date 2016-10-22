# Parameters
CXX = g++

BIN_DIR = bin
CLIENT = $(BIN_DIR)/RPC_FLock_client
SERVER = $(BIN_DIR)/RPC_FLock_server

SOURCES_CLNT_DIR = src/client
SOURCES_SVC_DIR = src/server
SOURCES_SHARED_DIR = src/shared

SOURCES_CLNT.c = $(shell find $(SOURCES_CLNT_DIR) -name '*.cpp')
SOURCES_CLNT.h = $(shell find $(SOURCES_CLNT_DIR) -name '*.hpp')
SOURCES_SVC.c = $(shell find $(SOURCES_SVC_DIR) -name '*.cpp')
SOURCES_SVC.h = $(shell find $(SOURCES_SVC_DIR) -name '*.hpp')
SOURCES_SHARED = $(shell find $(SOURCES_SHARED_DIR) -name '*.cpp')
SOURCES.x = rpcgen/RPC_FLock.x

OBJ_DIR = obj
OBJECTS_CLNT = $(SOURCES_CLNT.c:%.cpp=%.o) $(SOURCES_SHARED:%.cpp=%.o)
OBJECTS_SVC = $(SOURCES_SVC.c:%.cpp=%.o) $(SOURCES_SHARED:%.cpp=%.o)

# Compiler flags 
CXXFLAGS += -g -std=c++11
LDLIBS += -lnsl -lpthread

# Targets 
all : $(CLIENT) $(SERVER)

#all :
#	@echo $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCES_CLNT.c:%.cpp=%.o)))

$(OBJECTS_CLNT) : $(SOURCES_CLNT.c) $(SOURCES_CLNT.h) $(SOURCES_SHARED) 

$(OBJECTS_SVC) : $(SOURCES_SVC.c) $(SOURCES_SVC.h) $(SOURCES_SHARED) 

$(CLIENT) : $(OBJECTS_CLNT) 
	$(CXX)       -o $(CLIENT) $(OBJECTS_CLNT) $(LDLIBS) 

$(SERVER) : $(OBJECTS_SVC) 
	$(CXX)       -o $(SERVER) $(OBJECTS_SVC) $(LDLIBS)

clean:
	 $(RM) core $(OBJECTS_CLNT) $(OBJECTS_SVC) $(CLIENT) $(SERVER)

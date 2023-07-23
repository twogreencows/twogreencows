#declare variables
CC=g++
CFLAGS=-c -Wall -std=c++17 
LDFLAGS= $(libs) -luv -lsqlite3
LDFLAGSPOST = $(libsposts)
HEADER_PATH_RJSON="./third_parties/rapidjson-master/include"
HEADER_PATH_MDNS="./third_parties/mdns/include"
HEADER_PATH_LIBUV="./third_parties/libuv/include"

ifeq ($(shell uname -s), Darwin)
    libs = 
    libsposts = 
    PLATFORM_HEADER_PATH =
else
    libs = -luuid -lmmal -lmmal_core -lmmal_util 
    libsposts = -lstdc++fs
    PLATFORM_HEADER_PATH = "/opt/vc/include/"
    PLATFORM_LIB_PATH = "/opt/vc/lib/"
endif


$(info  = COMPILING TWOGREENCOWS AGENT =  )

BASE_DIR = $(strip $(CURDIR)) 

#Compile ddirectories
BUILD_DIR = $(addsuffix /build/ , $(BASE_DIR))
OBJ_DIR = $(addsuffix obj/, $(BUILD_DIR))
BIN_DIR = $(addsuffix bin/, $(BUILD_DIR))

MODULEDIR = $(addsuffix /modules/ ,$(BASE_DIR))
COREDIR = $(addsuffix /core/ ,$(BASE_DIR))
UTILDIR = $(addsuffix /utils/ ,$(BASE_DIR))
SRC:= $(wildcard *.cpp ./core/*.cpp ./modules/*.cpp ./utils/*.cpp)
OBJ_FILES = $(patsubst %cpp, %o,$(SRC))

#this target will create the software

all:compile
	$(CC)  $(LDFLAGS) -v $(OBJ_DIR)/*.o  -L$(PLATFORM_LIB_PATH) -o twogreencows_agent $(LDFLAGSPOST) 

#this target will compile only

compile:buildstructure
	$(CC) $(CFLAGS) -I$(COREDIR) -I$(MODULEDIR) -I$(UTILDIR) -I$(HEADER_PATH_RJSON) -I$(HEADER_PATH_MDNS) -I$(HEADER_PATH_LIBUV) -I$(PLATFORM_HEADER_PATH) $(SRC) 
	mv *.o $(OBJ_DIR)

buildstructure:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

#this target will clean everything
.PHONY:clean
clean:
	rm -rf *.o twogreencows_agent $(BUILD_DIR)

#declare variables
CC=g++
CFLAGS=-c -Wall -std=c++17 
LDFLAGS= $(libs) -luv -lsqlite3
LDFLAGSPOST = $(libsposts)
HEADER_PATH="./third_parties/rapidjson-master/include"

ifeq ($(shell uname -s), Darwin)
    libs = 
    libsposts = 
else
    libs = -luuid 
    libsposts = -lstdc++fs
endif


$(info    VAR is $(libsposts))
$(info    VAR is $(libsposts))

BASE_DIR = $(strip $(CURDIR)) 

#Compile ddirectories
BUILD_DIR = $(addsuffix /build/ , $(BASE_DIR))
OBJ_DIR = $(addsuffix /obj/, $(BUILD_DIR))
BIN_DIR = $(addsuffix /bin/, $(BUILD_DIR))

MODULEDIR = $(addsuffix /modules/ ,$(BASE_DIR))
COREDIR = $(addsuffix /core/ ,$(BASE_DIR))
SRC:= $(wildcard *.cpp ./core/*.cpp ./modules/*.cpp)
OBJ_FILES = $(patsubst %cpp, %o,$(SRC))

#this target will create the software

all:compile
	$(CC)  $(LDFLAGS) -v $(OBJ_DIR)/*.o  -o twogreencows_agent $(LDFLAGSPOST) 

#this target will compile only

compile:buildstructure
	$(CC) $(CFLAGS) -I$(COREDIR) -I$(MODULEDIR) -I$(HEADER_PATH) $(SRC) 
	mv *.o $(OBJ_DIR)

buildstructure:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

#this target will clean everything
.PHONY:clean
clean:
	rm -rf *.o twogreencows_agent $(BUILD_DIR)

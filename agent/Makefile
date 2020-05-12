#declare variables
CC=g++
CFLAGS=-c -Wall

#this target will create the software

all:compile
	$(CC) main.o -o twogreencows_agent

#this target will compile only
compile:
	$(CC) $(CFLAGS) main.cpp

#this target will clean everything
clean:
	rm -rf *.o twogreencows_agent

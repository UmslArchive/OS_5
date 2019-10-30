#Author:   Colby Ackerman
#Class:    Operating Systems - CS4760
#Assign:   #5
#Date:     11/5/19
#=======================================================

CC = gcc
CFLAGS = -I. -g
OBJECTS = shared.o
.SUFFIXES: .c .o

all: oss usrPs

oss: oss.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ oss.o $(OBJECTS) -lpthread

usrPs: userPs.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ userPs.o $(OBJECTS) -lpthread

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o oss usrPs *.txt
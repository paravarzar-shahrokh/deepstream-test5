CC=gcc
CFLAGS=-Wall

simple: json_demo_simple.o mjson.o
	$(CC) $(CFLAGS) -o json_demo_simple json_demo_simple.o mjson.o
compound: json_demo_compound.o mjson.o
	$(CC) $(CFLAGS) -o json_demo_compound json_demo_compound.o mjson.o

all: json_demo_simple.o json_demo_compound.o mjson.o
	$(CC) $(CFLAGS) -o json_demo_simple json_demo_simple.o mjson.o
	$(CC) $(CFLAGS) -o json_demo_compound json_demo_compound.o mjson.o

clean:
	rm -rf json_demo_simple json_demo_compound json_demo_simple.o json_demo_compound.o mjson.o

json_demo_simple.o: json_demo_simple.c mjson.h
json_demo_compound.o: json_demo_compound.c mjson.h
mjson.o: mjson.c mjson.h

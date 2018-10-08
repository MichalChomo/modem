CC=g++
CFLAGS=-std=c++11 -Wall -pedantic -O2 -no-pie
LIBS=-lm

all: bms2A bms2B


bms2A: bms2A.cpp
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@ libsndfile.a

bms2B: bms2B.cpp
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@ libsndfile.a

clean:
	rm -f *.o bms2A bms2B

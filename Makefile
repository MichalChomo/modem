CC=g++
CFLAGS=-std=c++11 -Wall -pedantic -O2 -no-pie
LIBS=-lm
OBJS=FileNameHelper.o

all: bms1A bms1B

%.o: %.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

bms1A: bms1A.cpp $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@ libsndfile.a

bms1B: bms1B.cpp
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@ libsndfile.a

clean:
	rm -f *.o bms1A bms1B
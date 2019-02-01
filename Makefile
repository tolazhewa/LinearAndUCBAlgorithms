CC=g++
CFLAGS = --std=c++11
CLASSES = Arm.cpp Environment.cpp
Q1_CLASSES = UCBAgent.cpp
Q2_CLASSES = LRAgent.cpp

all: q1 q2

q1: q1.cpp
	$(CC) -o q1.o q1.cpp $(Q1_CLASSES) $(CLASSES) $(CFLAGS)

q2: q2.cpp
	$(CC) -o q2.o q2.cpp $(Q2_CLASSES) $(CLASSES) $(CFLAGS)

clean:
	rm *.o
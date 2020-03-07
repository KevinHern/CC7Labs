all: 
	gcc -lpthread createThread.c -o createThread.o
	gcc -lpthread joinThread.c -o joinThread.o
	gcc -lpthread mutexThread.c -o mutexThread.o
	gcc -lpthread conditionThread.c -o conditionThread.o

brige:
	gcc -lpthread brige.c -o brige.o && ./brige.o

clean: 
	rm -f *.o
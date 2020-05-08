G=g++
GFLAGS=-std=c++11 -Wpedantic -Wall -Wextra -g

csim: main.o CacheSim.o
	$(G) main.o CacheSim.o -o csim

main.o: main.cpp CacheSim.h
	$(G) $(GFLAGS) -c main.cpp

CacheSim.o: CacheSim.cpp CacheSim.h
	$(G) $(GFLAGS) -c CacheSim.cpp

clean:
	-rm -f *.o csim

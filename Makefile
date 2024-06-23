all: randomGenerator primeCounter

randomGenerator: generator.c
	gcc -o randomGenerator generator.c -lpthread

primeCounter: primeCounter.c
	gcc -o primeCounter primeCounter.c -lpthread

clean:
	rm -f randomGenerator primeCounter

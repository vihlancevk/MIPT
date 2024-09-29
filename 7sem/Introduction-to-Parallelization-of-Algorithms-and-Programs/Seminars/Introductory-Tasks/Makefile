first:
	gcc -fopenmp -o first.sh first.c
	./first.sh
	rm first.sh

N ?= 10
second:
	gcc -fopenmp -o second.sh second.c
	./second.sh $(N)
	rm second.sh

third:
	gcc -fopenmp -o third.sh third.c
	./third.sh $(N)
	rm third.sh

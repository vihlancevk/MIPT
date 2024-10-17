s:
	gcc -fopenmp -O3 s.c -lm
	./a.out
	rm a.out

p:
	mpicc -O3 -o a.out p.c -lm
	mpirun -np 3 a.out
	rm a.out

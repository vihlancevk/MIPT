s:
	gcc -fopenmp -O3 s.c ../square-matrix-multiplication/matrix/matrix.c
	./a.out left.txt right.txt s.txt
	rm a.out

p:
	gcc -fopenmp -O3 p.c /usr/lib/x86_64-linux-gnu/libOpenCL.so.1 ../square-matrix-multiplication/matrix/matrix.c
	./a.out left.txt right.txt p.txt
	rm a.out

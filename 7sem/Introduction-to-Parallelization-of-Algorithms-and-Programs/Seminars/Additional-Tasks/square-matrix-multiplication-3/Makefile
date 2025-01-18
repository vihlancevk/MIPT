t:
	gcc -fopenmp -fcf-protection=none -no-pie -o t.out test.c
	./t.out
	rm t.out

s:
	gcc -fopenmp -O3 -o s.out s.c ../square-matrix-multiplication/matrix/matrix.c
	./s.out left.txt right.txt s.txt
	rm s.out

s-t:
	gcc -fopenmp -O3 -o s-t.out s-t.c ../square-matrix-multiplication/matrix/matrix.c
	./s-t.out left.txt right.txt s-t.txt
	rm s-t.out

p:
	gcc -fopenmp -fcf-protection=none -fno-lto -o p.out p.c ../square-matrix-multiplication/matrix/matrix.c
	./p.out left.txt right.txt p.txt
	rm p.out

p-t:
	gcc -fopenmp -fcf-protection=none -fno-lto -o p-t.out p-t.c ../square-matrix-multiplication/matrix/matrix.c
	./p-t.out left.txt right.txt p-t.txt
	rm p-t.out

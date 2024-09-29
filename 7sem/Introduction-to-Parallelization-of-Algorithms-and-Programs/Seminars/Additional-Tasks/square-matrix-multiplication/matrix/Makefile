n ?= 3
m ?= 3
matrix-file ?= matrix.txt
test-matrix:
	gcc -o test-matrix.sh test-matrix.c matrix.c
	./test-matrix.sh $(n) $(m) $(matrix-file)
	rm ./test-matrix.sh

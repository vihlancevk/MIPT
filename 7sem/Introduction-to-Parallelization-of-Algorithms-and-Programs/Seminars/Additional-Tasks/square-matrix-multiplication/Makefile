n-dsos ?= 3
left-matrix-file-dsos ?= tests/left.txt
right-matrix-file-dsos ?= tests/right.txt
result-matrix-file-dsos ?= tests/result.txt
debug-simply-optimized-sequential-algorithm:
	gcc -DDEBUG -fopenmp -O3 -o simply-optimized-sequential-algorithm.sh simply-optimized-sequential-algorithm.c matrix/matrix.c
	valgrind -s ./simply-optimized-sequential-algorithm.sh $(n-dsos) $(left-matrix-file-dsos) $(right-matrix-file-dsos) $(result-matrix-file-dsos)
	rm ./simply-optimized-sequential-algorithm.sh

n-sos ?= 32
left-matrix-file-tsos ?= data/left-matrix.txt
right-matrix-file-tsos ?= data/right-matrix.txt
result-matrix-file-tsos ?= data/result-matrix.txt
simply-optimized-sequential-algorithm:
	gcc -fopenmp -O3 -o simply-optimized-sequential-algorithm.sh simply-optimized-sequential-algorithm.c matrix/matrix.c
	./simply-optimized-sequential-algorithm.sh $(n-sos) $(left-matrix-file-tsos) $(right-matrix-file-tsos) $(result-matrix-file-tsos)
	rm ./simply-optimized-sequential-algorithm.sh

n-sop := 32
left-matrix-file-tsop := data/left-matrix.txt
right-matrix-file-tsop := data/right-matrix.txt
result-matrix-file-tsop := data/result-matrix.txt
simply-optimized-parallel-algorithm:
	gcc -fopenmp -O3 -o simply-optimized-parallel-algorithm.sh simply-optimized-parallel-algorithm.c matrix/matrix.c
	./simply-optimized-parallel-algorithm.sh $(n-sop) $(left-matrix-file-tsop) $(right-matrix-file-tsop) $(result-matrix-file-tsop)
	rm ./simply-optimized-parallel-algorithm.sh

n-os := 32
left-matrix-file-tos := data/left-matrix.txt
right-matrix-file-tos := data/right-matrix.txt
result-matrix-file-tos := data/result-matrix.txt
optimized-sequential-algorithm:
	gcc -fopenmp -O3 -o optimized-sequential-algorithm.sh optimized-sequential-algorithm.c matrix/matrix.c
	./optimized-sequential-algorithm.sh $(n-os) $(left-matrix-file-tos) $(right-matrix-file-tos) $(result-matrix-file-tos)
	rm ./optimized-sequential-algorithm.sh

n-op := 32
left-matrix-file-top := data/left-matrix.txt
right-matrix-file-top := data/right-matrix.txt
result-matrix-file-top := data/result-matrix.txt
optimized-parallel-algorithm:
	gcc -fopenmp -O3 -o optimized-parallel-algorithm.sh optimized-parallel-algorithm.c matrix/matrix.c
	./optimized-parallel-algorithm.sh $(n-op) $(left-matrix-file-top) $(right-matrix-file-top) $(result-matrix-file-top)
	rm ./optimized-parallel-algorithm.sh

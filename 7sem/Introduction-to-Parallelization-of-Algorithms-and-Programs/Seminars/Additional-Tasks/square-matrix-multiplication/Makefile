n-dsos ?= 3
left-matrix-file-dsos ?= tests/left.txt
right-matrix-file-dsos ?= tests/right.txt
result-matrix-file-dsos ?= tests/result.txt
debug-simply-optimized-sequential-algorithm:
	gcc -DDEBUG -o simply-optimized-sequential-algorithm.sh simply-optimized-sequential-algorithm.c matrix/matrix.c
	valgrind -s ./simply-optimized-sequential-algorithm.sh $(n-dsos) $(left-matrix-file-dsos) $(right-matrix-file-dsos) $(result-matrix-file-dsos)
	rm ./simply-optimized-sequential-algorithm.sh

n-tsos ?= 3
left-matrix-file-tsos ?= data/left-matrix.txt
right-matrix-file-tsos ?= data/right-matrix.txt
result-matrix-file-tsos ?= data/result-matrix.txt
time-simply-optimized-sequential-algorithm:
	gcc -fopenmp -o simply-optimized-sequential-algorithm.sh simply-optimized-sequential-algorithm.c matrix/matrix.c
	/usr/bin/time -f "%E" ./simply-optimized-sequential-algorithm.sh $(n-tsos) $(left-matrix-file-tsos) $(right-matrix-file-tsos) $(result-matrix-file-tsos)
	rm ./simply-optimized-sequential-algorithm.sh

n-tsop := 3
left-matrix-file-tsop := data/left-matrix.txt
right-matrix-file-tsop := data/right-matrix.txt
result-matrix-file-tsop := data/result-matrix.txt
time-simply-optimized-parallel-algorithm:
	gcc -fopenmp -o simply-optimized-parallel-algorithm.sh simply-optimized-parallel-algorithm.c matrix/matrix.c
	/usr/bin/time -f "%E" ./simply-optimized-parallel-algorithm.sh $(n-tsop) $(left-matrix-file-tsop) $(right-matrix-file-tsop) $(result-matrix-file-tsop)
	rm ./simply-optimized-parallel-algorithm.sh

n-dos := 3
left-matrix-file-dos = tests/left.txt
right-matrix-file-dos = tests/right.txt
result-matrix-file-dos = tests/result.txt
debug-optimized-sequential-algorithm:
	gcc -DDEBUG -o optimized-sequential-algorithm.sh optimized-sequential-algorithm.c matrix/matrix.c
	valgrind -s ./optimized-sequential-algorithm.sh $(n-dos) $(left-matrix-file-dos) $(right-matrix-file-dos) $(result-matrix-file-dos)
	rm ./optimized-sequential-algorithm.sh

n-tos := 3
left-matrix-file-tos := data/left-matrix.txt
right-matrix-file-tos := data/right-matrix.txt
result-matrix-file-tos := data/result-matrix.txt
time-optimized-sequential-algorithm:
	gcc -o optimized-sequential-algorithm.sh optimized-sequential-algorithm.c matrix/matrix.c
	/usr/bin/time -f "%E" ./optimized-sequential-algorithm.sh $(n-tos) $(left-matrix-file-tos) $(right-matrix-file-tos) $(result-matrix-file-tos)
	rm ./optimized-sequential-algorithm.sh

n-top := 3
left-matrix-file-top := data/left-matrix.txt
right-matrix-file-top := data/right-matrix.txt
result-matrix-file-top := data/result-matrix.txt
time-optimized-parallel-algorithm:
	gcc -fopenmp -o optimized-parallel-algorithm.sh optimized-parallel-algorithm.c matrix/matrix.c
	/usr/bin/time -f "%E" ./optimized-parallel-algorithm.sh $(n-top) $(left-matrix-file-top) $(right-matrix-file-top) $(result-matrix-file-top)
	rm ./optimized-parallel-algorithm.sh

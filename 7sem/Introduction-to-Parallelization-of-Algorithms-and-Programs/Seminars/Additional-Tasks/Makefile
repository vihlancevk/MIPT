nested:
	gcc -fopenmp -o nested.sh nested.c
	./nested.sh
	rm nested.sh

number-of-processor-cores:
	gcc -fopenmp -o number-of-processor-cores.sh number-of-processor-cores.c
	./number-of-processor-cores.sh
	rm number-of-processor-cores.sh

st ?= s
cs ?= 1
scheduling:
	gcc -fopenmp -o scheduling.sh scheduling.c
	./scheduling.sh $(st) $(cs)
	rm scheduling.sh

variable-visibility-scopes:
	gcc -fopenmp -o variable-visibility-scopes.sh variable-visibility-scopes.c
	./variable-visibility-scopes.sh
	rm variable-visibility-scopes.sh

t:
	gcc -fopenmp -fcf-protection=none -no-pie -o t.out test.c
	./t.out
	rm t.out

s:
	gcc -fopenmp -fcf-protection=none -o s.out s.c
	./s.out
	rm s.out

p:
	gcc -fopenmp -fcf-protection=none -fno-lto -o p.out p.c
	./p.out
	rm p.out

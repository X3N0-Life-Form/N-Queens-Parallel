
out: omp mpi

omp: n-queens-openMP.out

mpi: n-queens-mpi.out

n-queens-openMP.out: omp/n-queens-omp.cpp commons.o
	g++ $< -o $@ -fopenmp -std=c++11 -O3 commons.o

n-queens-mpi.out: mpi/n-queens-mpi.cpp commons.o
	mpic++ $< -o $@ -std=c++11 commons.o

commons.o: commons.cpp
	g++ $< -c


clean:
	rm -rf *.out
	rm -rf *.o

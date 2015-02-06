
out: omp

omp: n-queens-openMP.out

n-queens-openMP.out: omp/n-queens-omp.cpp
	g++ $< -o $@ -fopenmp -std=c++11 -O3

clean:
	rm -rf *.out
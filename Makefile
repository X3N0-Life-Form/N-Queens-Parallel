


n-queens-openMP.out: omp/n-queens-omp.cpp
	g++ $< -o $@ -fopenmp

clean:
	rm -rf *.out
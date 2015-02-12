# N-Queens-Parallel
codename glowing-wight; n-queens parallel solver

### Compilation
* `make`: génère les exécutables
    * n-queens-omp.out
    * n-queens-mpi.out

### Notes
##### OpenMP
* En lancement séquentiel, on va plus vite qu'en parallèle sur de petites instances.

##### MPI
* Affiche un message d'erreur puisque l'on utilise *MPI_ABORT*




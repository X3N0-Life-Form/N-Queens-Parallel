# N-Queens-Parallel
codename glowing-wight; n-queens parallel solver

### Compilation
* `make`: génère les exécutables
    * n-queens-omp.out
    * n-queens-mpi.out

### Exécution
* `./n-queens-omp.out [N] [type lancement]`
   * N = taille de l'échiquier (par défaut 8)
   * [type de lancement] = `-s` ou `-p` pour effectuer un lancement séquentiel ou parallèle (par défaut parallèle)
* `mpiexec -n 4 ./n-queens-mpi.out [N]`
   * N = taille de l'échiquier (par défaut 8)

### Notes
##### OpenMP
* En lancement séquentiel, on va plus vite qu'en parallèle sur de petites instances.

##### MPI
* Affiche un message d'erreur puisque l'on utilise *MPI_ABORT*




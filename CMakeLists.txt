cmake_minimum_required(VERSION 2.8)

project("N-Queens-Parallel")

#
# Commons
#

set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_CXX_FLAGS "-std=c++0x")

add_definitions(
  -Wall
  -O2
)

set(NUM_THREADS
  8
)

#
# MPI
#

set(MPI_SRC
  mpi/n-queens-mpi.cpp
)

#add_executable(n-queens-mpi.out
#  ${MPI_SRC}
#)

add_custom_target(n-queens-mpi.out
  mpic++ ${MPI_SRC} -o n-queens-mpi.out
)

add_custom_target(run_mpi
  mpiexec -n ${NUM_THREADS} n-queens-mpi
)

#
# OpenMP
#

set(OMP_SRC
  omp/n-queens-omp.cpp
)

set(OMP_REQ
  -fopenmp
)

add_executable(n-queens-openMP.out
  ${OMP_SRC}
)
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

#
# MPI
#

set(MPI_SRC
  mpi/n-queens.cpp
)

add_executable(n-queens-mpi
  ${MPI_SRC}
)

add_custom_target(run_mpi
  mpiexec -n ${NUM_THREADS} n-queens-mpi
)

#
# OpenMP
#

set(OMP_SRC
  omp/n-queens.cpp
)

set(OMP_REQ
  -fopenmp
)

add_executable(n-queens-openMP
  ${OMP_SRC}
)
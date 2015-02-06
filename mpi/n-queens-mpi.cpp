#include <mpi.h>

int size = 8;

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);

  MPI_Finalize();
}

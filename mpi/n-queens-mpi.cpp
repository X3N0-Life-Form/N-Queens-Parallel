#include <mpi.h>
#include <iostream>

#include "../commons.h"

using namespace std;

int size = 8;
bool flase = false;
int* global_queens;
bool firstLoop = true;

bool stop() {
  /*if (firstLoop) {
    firstLoop = false;
    return false;
  }
  int tid;
  MPI_Comm_rank(MPI_COMM_WORLD, &tid);
  int numberOfThreads;
  MPI_Comm_size(MPI_COMM_WORLD, &numberOfThreads);
  for (int i = 0; i <numberOfThreads; i++) {
    if (tid != i)
      MPI_Recv(&flase, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }*/
  return flase;
}

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);

  if (argc > 1) {
    size = atoi(argv[1]);
  }
  int tid;
  MPI_Comm_rank(MPI_COMM_WORLD, &tid);
  int numberOfThreads;
  MPI_Comm_size(MPI_COMM_WORLD, &numberOfThreads);
  int** boards;
  int* costs;
  int* local_queens;

  srand(time(NULL) + tid * numberOfThreads);
  if (tid == 0) {
    global_queens = new int[size];
    generateBoard(global_queens);

    costs = new int[numberOfThreads];
    for (int i = 0; i < numberOfThreads; i++) costs[i] = -1;

    // transmit data
    for (int i = 1; i < numberOfThreads; i++) {
      MPI_Send(global_queens, size, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
    local_queens = global_queens;
  } else {
    // receive data
    local_queens = new int[size];
    MPI_Recv(local_queens, size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  int cost = descent_2(local_queens);
  if (cost == 0) {
    flase = true;
    cout << "Found a cost = 0." << endl;
    MPI_Abort(MPI_COMM_WORLD, MPI_SUCCESS);
  }
  //cout << "broadcast " << flase << endl;
  //MPI_Bcast(&flase, 1, MPI_INT, tid, MPI_COMM_WORLD);

  // job done, send the cost to thread 0
  if (tid == 0) {
    costs[0] = cost;
    for (int i = 1; i < numberOfThreads; i++) {
      MPI_Recv(&costs[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (int i = 0; i < numberOfThreads; i++) {
      cout << "Final board with cost=" << costs[i] << ":\n";
    }
  } else {
    // send back our findings
    MPI_Send(&cost, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  
  MPI_Finalize();
}

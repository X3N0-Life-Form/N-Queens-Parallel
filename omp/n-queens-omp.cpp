#include <omp.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include "../commons.h"

using namespace std;

int* global_queens;
int size = 8;
bool flase = false;

void shuffleBoard(int* queens) {
  omp_set_num_threads(size);
#pragma omp parallel
  //for (int i = 0; i < size; i++)
  {
    int i = omp_get_thread_num();
    queens[i] = rand() % size;
  }
}

void shuffleQueen(int q, int* queens) {
  queens[q] = rand() % size;
}

bool stop() {
  return flase;
}

enum e_descent_t {
  STANDARD,
  PARALLEL
};
typedef e_descent_t e_descent;

e_descent getDescentType(char* type) {
  if (!strcmp(type,"-s")) {
    return STANDARD;
  } else if (!strcmp(type, "-p")) {
    return PARALLEL;
  } else {
    return PARALLEL;
  }
}

int main(int argc, char** argv) {
 // srand(1);
  e_descent descent_type = PARALLEL;
  if (argc > 1) {
    size = atoi(argv[1]);
  }
  if (argc > 2) {
    descent_type = getDescentType(argv[2]);
  }

  global_queens = new int[size];

  generateBoard(global_queens);
  int** boards = new int*[NUMBER_OF_THREADS];
  boards[0] = global_queens;
  for (int i = 1; i < NUMBER_OF_THREADS; i++) {
    boards[i] = copyBoard(global_queens);
  }
  int64_t* costs = new int64_t[NUMBER_OF_THREADS];
  for (int i = 0; i < NUMBER_OF_THREADS; i++) costs[i] = -1;

  switch (descent_type) {
  case STANDARD:
    cout << "Entering sequential descent..." << endl;
    for (int i; i < NUMBER_OF_THREADS; i++) {
      costs[i] =  descent_2(boards[i]);
      if (costs[i] == 0) break;
    }
    break;
  case PARALLEL:
    cout << "Entering parallel descent..." << endl;
    omp_set_num_threads(NUMBER_OF_THREADS);
#pragma omp parallel
    {
      int tid = omp_get_thread_num();
      costs[tid] =  descent_2(boards[tid]);
      if (costs[tid] == 0) flase = true;
    }
    break;
  }
  

  cout << "\n\n";
  for (int i = 0; i < NUMBER_OF_THREADS; i++) {
    //printBoard(boards[i]);
    cout << "Final board with cost=" << costs[i] << ":\n";
  }

  delete[] global_queens;
}

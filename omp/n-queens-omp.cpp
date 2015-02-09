#include <omp.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include "../commons.h"

using namespace std;

int* global_queens;
int size = 8;


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



bool flase = false;

int64_t descent(int* queens) {
 
  int64_t cost = calculateCost(queens);
  int q1 = 0;
  int q2 = 0;

  for (int i = 0; i < 1000000; i++) {
    if (cost > 0 && !flase) {
      q1 = rand() % size;
      q2 = rand() % size;
      while (q2 == q1)  q2 = rand() % size;
      int64_t tempCost = cost;
      tempCost -= updated_cost(queens, q1, q2);
      swap(q1, q2, queens);
      tempCost += updated_cost(queens, q1, q2);

      if (tempCost > cost) {
        	swap(q1, q2, queens);
      } else {
      	cost = tempCost;
      }
    } else {
      break;
    }
  }
  return cost;
}


int64_t descent_2(int* queens) {
 
  int64_t cost = calculateCost(queens);
  int q1 = 0;
  int q2 = 0;

  for (int i = 0; i < 1000000; i++) {
    if (cost > 0 && !flase) {
      if (cost > 3) q1 = rand() % size;
      else q1 = getFirstConflict(queens);
      q2 = rand() % size;
      while (q2 == q1)  q2 = rand() % size;
      int64_t tempCost = cost;
      tempCost -= updated_cost(queens, q1, q2);
      swap(q1, q2, queens);
      tempCost += updated_cost(queens, q1, q2);

      if (tempCost > cost) {
          swap(q1, q2, queens);
      } else {
        cost = tempCost;
      }
    } else {
      break;
    }
  }
  return cost;
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

#include <omp.h>
#include <cstdlib>
#include <iostream>
#include <cstdint>
#include <cstring>
#include "../commons.h"

using namespace std;

int* global_queens;
int size = 8;


/**
 * @return true if the specified queen doesn't have any conflict.
 */
bool checkQueen(int q, int* queens) {
  for (int i = 0; i < size; i++)
  {
    if (i != q) {
      if (queens[i] == queens[q]) {
	return false;
      }

      int diff = abs(i - q);
      if (abs(queens[i] - queens[q]) == diff) {
	return false;
      }
    
    }
  }
  return true;
}

/**
 * @return true if the entire board is OK.
 */
bool checkBoard(int* queens) {
  for (int i = 0; i < size; i ++) {
    if (!checkQueen(i, queens)) {
      return false;
    }
  }
  return true;
}

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

void swap(int q1, int q2, int* queens) {
  int tmp = queens[q1];
  queens[q1] = queens[q2];
  queens[q2] = tmp;
}

int* copyBoard(int* queens) {
  int* nuBoard = new int[size];
  for (int i = 0; i < size; i++) {
    nuBoard[i] = queens[i];
  }
  return nuBoard;
}


int64_t  lineConflict(int iValue, int jValue) {
  if (iValue == jValue){
    return 1;
  }
  return 0;
}

int64_t  diagConflict(int difference, int iValue, int jValue) {
  if (difference == abs(iValue - jValue)) {
    return 1;
  }
  return 0;
}

int64_t calculateCost(int* queens, int* conflicts = NULL) {
  int64_t  cost = 0;
  int c_i = 0;
  for(int i = 0; i < size; i++){
    for (int j = i + 1 ; j < size ; j++){
      cost += diagConflict(j-i, queens[i], queens[j]) ;
      cost += lineConflict(queens[i], queens[j]);
      if (conflicts != NULL && (c_i == 0 || conflicts[c_i - 1] != i)) {
	conflicts[c_i] = i;
	c_i++;
      }
    }
  }
  return cost;
}

int64_t updated_cost(int * queens, int index1, int index2) {
  int64_t  toReturn = 0;
  for(int i = 0; i < size; i++) {
    if (i!=index1) {
      toReturn+= diagConflict(abs(i-index1), queens[index1], queens[i]);
      toReturn+=lineConflict(queens[index1], queens[i]);
    }
    if (i!=index2) {
      toReturn+= diagConflict(abs(i-index2), queens[index2], queens[i]);
      toReturn+=lineConflict(queens[index2], queens[i]);
    }
  }

  return toReturn;
}


int64_t descent_it_it(int* queens) {
 
  int64_t cost = calculateCost(queens);

  for (int i = 0; i < 50 * 380; i++) {
    if (cost > 0) {
      int q1 = rand() % size;
      int q2 = rand() % size;
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
  if (strcmp(type, "-s")) {
    return STANDARD;
  } else if (strcmp(type, "-p")) {
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
  // insert board & cost arrays here
  switch (descent_type) {
  case STANDARD:
    //call regular descent here
    break;
  case PARALLEL:
    //call parallel descent here
    break;
  }
  // handle results here

  global_queens = new int[size];

  generateBoard(global_queens);

  int64_t cost =  descent_it_it(global_queens);
  
  cout << "\n\n\n";
  printBoard(global_queens);
  cout << "Final board with cost=" << cost << ":\n";

  delete[] global_queens;
}

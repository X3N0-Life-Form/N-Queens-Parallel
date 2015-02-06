#include <omp.h>
#include <cstdlib>
#include <iostream>
#include <cstdint>

using namespace std;

int* global_queens;
int size = 8;
int level = 6;


void printBoard(int* queens) {
  for (int z = 0; z < size; z++) cout << queens[z] << " ";
  cout << endl;
}

void generateBoard(int* queens) {
  omp_set_num_threads(size);
  cout << "Generating board of size=" << size << endl;
  //#pragma omp parallel
  for (int i = 0; i < size; i++)
  {
    //int i = omp_get_thread_num();
    queens[i] = i;
  }
}

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
    //  cost += lineConflict(queens[i], queens[j]);
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
     // toReturn+=lineConflict(queens[index1], queens[i]);
    }
    if (i!=index2) {
      toReturn+= diagConflict(abs(i-index2), queens[index2], queens[i]);
     // toReturn+=lineConflict(queens[index2], queens[i]);
    }
  }

  return toReturn;
}

int getFirstConflict(int* queens) {
  for (int i = 0; i < size - 1; i++) {
    for (int j = i + 1; j < size; j++) {
      if(diagConflict(j-i, queens[i], queens[j]) == 1) {
        return i;
      }
    }
  }
  return -1;
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


int main(int argc, char** argv) {
 // srand(1);
  if (argc > 1) {
    size = atoi(argv[1]);
  }

  global_queens = new int[size];

  generateBoard(global_queens);
  int** boards = new int*[4];
  boards[0] = global_queens;
  for (int i = 1; i < 4; i++) {
    boards[i] = copyBoard(global_queens);
  }

  omp_set_num_threads(4);
  int64_t* costs = new int64_t[4];
#pragma omp parallel
  {
    int tid = omp_get_thread_num();
    costs[tid] =  descent_2(boards[tid]);
    if (costs[tid] == 0) flase = true;
  }
  
  cout << "\n\n\n";
  for (int i = 0; i < 4; i++) {
    //printBoard(boards[i]);
    cout << "Final board with cost=" << costs[i] << ":\n";
  }

  delete[] global_queens;
}

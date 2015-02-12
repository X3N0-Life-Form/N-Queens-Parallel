#include "commons.h"

#include <iostream>

using namespace std;

extern int size; // declared in the n-queens main source files
extern bool flase;

void printBoard(int* queens) {
  for (int z = 0; z < size; z++) cout << queens[z] << " ";
  cout << endl;
}

void generateBoard(int* queens) {
  cout << "Generating board of size=" << size << endl;
  for (int i = 0; i < size; i++) {
    queens[i] = i;
  }
}

int* copyBoard(int* queens) {
  int* nuBoard = new int[size];
  for (int i = 0; i < size; i++) {
    nuBoard[i] = queens[i];
  }
  return nuBoard;
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



int64_t lineConflict(int iValue, int jValue) {
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

int64_t calculateCost(int* queens, int* conflicts) {
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

void swap(int q1, int q2, int* queens) {
  int tmp = queens[q1];
  queens[q1] = queens[q2];
  queens[q2] = tmp;
}


/////////////////////////
/// Descent functions ///
/////////////////////////

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

extern bool stop();

int64_t descent_2(int* queens) {
 
  int64_t cost = calculateCost(queens);
  int q1 = 0;
  int q2 = 0;

  for (int i = 0; i < 1000000; i++) {
    if (cost > 0 && !stop()) {
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

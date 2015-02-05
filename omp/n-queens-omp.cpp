#include <omp.h>
#include <cstdlib>
#include <iostream>
#include <chrono>

using namespace std;

int* global_queens;
int size = 8;
int level = 6;
bool stop = false;
int depth = 0;
int maxDepth = 10000;


std::string getRunTimeString(chrono::steady_clock::time_point beginTime, chrono::steady_clock::time_point endTime) {
  std::string res;
  std::chrono::steady_clock::duration diff = endTime - beginTime;
  auto h = std::chrono::duration_cast<std::chrono::hours> (diff);
  auto min = std::chrono::duration_cast<std::chrono::minutes> (diff);
  auto sec = std::chrono::duration_cast<std::chrono::seconds> (diff);
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds> (diff);
  auto ns = std::chrono::duration_cast<std::chrono::nanoseconds> (diff);
  if (h.count() > 0) {
    res.append(std::to_string(h.count()));
    res.append("h ");
  }
  if (min.count() > 0) {
    res.append(std::to_string(min.count() % 60));
    res.append(" min ");
  }
  res.append(std::to_string(sec.count() % 60));
  res.append(".");
  if (ms.count() % 1000 < 100)
    res.append("0");
  if (ms.count() % 1000 < 10)
    res.append("0");
  res.append(std::to_string(ms.count() % 1000));
  if (ms.count() == 0) {
    if (ns.count() % 1000 < 100)
      res.append("0");
    if (ns.count() % 1000 < 10)
      res.append("0");
    res.append(std::to_string(ns.count() % 1000));
  }
  res.append(" sec");
  return res;
}

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

void search_exhaustive(int* queens, int q) {
  if (q == size) {
    //stop
    if (checkBoard(queens)) {
      cout << "FINAL STOP:\t";
      printBoard(queens);
    }
  } else {
    for (int i = 0; i < size; i++) {
      if (stop) return;
      queens[q] = i;
      if (!checkBoard(queens)) {
	if (q < level) {
	  int* nuBoard = copyBoard(queens);
#pragma omp task 
	  search_exhaustive(nuBoard, q+1);
	} else {
	  search_exhaustive(queens, q+1);
	}
      } else {
	cout << "REGULAR STOP:\t";
	printBoard(queens);
	delete[] global_queens;
	global_queens = queens;
	stop = true;
      }
    }
  }
}


int lineConflict(int iValue, int jValue) {
  if (iValue == jValue){
    return 1;
  }
  return 0;
}

int diagConflict(int difference, int iValue, int jValue) {
  if (difference == abs(iValue - jValue)) {
    return 1;
  }
  return 0;
}

int calculateCost(int* queens, int* conflicts = NULL) {
  int cost = 0;
  int c_i = 0;
  for(int i = 0; i < size; i++){
    for (int j = i + 1 ; j < size ; j++){
      cost += diagConflict(j-i, queens[i], queens[j]) ;
      //cost += lineConflict(queens[i], queens[j]);
      if (conflicts != NULL && (c_i == 0 || conflicts[c_i - 1] != i)) {
	conflicts[c_i] = i;
	c_i++;
      }
    }
  }
  return cost;
}

bool shouldWeStop() {
  return depth >= maxDepth;
}

void descent(int* queens) {
  int cost = calculateCost(queens);
  if (cost > 0 && !stop) {
    int q1 = rand() % size;
    int q2 = rand() % size;
    while (q2 == q1)  q2 = rand() % size;
    swap(q1, q2, queens);
    int tempCost = calculateCost(queens);
    if (tempCost > cost) {
      swap(q1, q2, queens);
    } else {
      cost = tempCost;
    }
    depth++;
    stop = shouldWeStop();
    //cout << "depth=" << depth << "; cost=" << cost << endl;
    #pragma omp task
    descent(queens);
  }
}


void descent_it(int* queens) {
  for (int k = 0; k < 50; k++) {
  //for (int i = 0; i < 50 * 380; i++)
    omp_set_num_threads(380);
#pragma omp parallel
    {
      int i = omp_get_thread_num();
      int cost = calculateCost(queens);
      if (cost > 0 && !stop) {
	int q1 = rand() % size;
	int q2 = rand() % size;
	while (q2 == q1)  q2 = rand() % size;
	swap(q1, q2, queens);
	int tempCost = calculateCost(queens);
	if (tempCost > cost) {
	  swap(q1, q2, queens);
	} else {
	  cost = tempCost;
	}
	depth++;
	//stop = shouldWeStop();
	//cout << "depth=" << depth << "; cost=" << cost << endl;
      } else {
	//break;
      }
    }
  }
}

void descent_it_it(int* queens) {
  for (int i = 0; i < 50 * 380; i++) {
    int cost = calculateCost(queens);
    if (cost > 0 && !stop) {
      int q1 = rand() % size;
      int q2 = rand() % size;
      while (q2 == q1)  q2 = rand() % size;
      swap(q1, q2, queens);
      int tempCost = calculateCost(queens);
      if (tempCost > cost) {
	swap(q1, q2, queens);
      } else {
	cost = tempCost;
      }
      depth++;
      //stop = shouldWeStop();
      //cout << "depth=" << depth << "; cost=" << cost << endl;
    } else {
      //break;
    }
  }
}

void descent_it_conf(int* queens) {
  for (int k = 0; k < 50; k++) {
  //for (int i = 0; i < 50 * 380; i++)
    omp_set_num_threads(380);
#pragma omp parallel
    {
      int i = omp_get_thread_num();
      int* conflicts = new int[size];
      int cost = calculateCost(queens, conflicts);
      if (cost > 0 && !stop) {
	int q1 = conflicts[rand() % cost];
	int q2 = rand() % size;
	while (q2 == q1)  q2 = rand() % size;
	swap(q1, q2, queens);
	int tempCost = calculateCost(queens);
	if (tempCost > cost) {
	  swap(q1, q2, queens);
	} else {
	  cost = tempCost;
	}
	depth++;
	//stop = shouldWeStop();
	//cout << "depth=" << depth << "; cost=" << cost << endl;
      } else {
	//break;
      }
    }
  }
}



int main(int argc, char** argv) {
  srand(1);
  if (argc > 1) {
    size = atoi(argv[1]);
  }
  global_queens = new int[size];
  generateBoard(global_queens);
  printBoard(global_queens);

  std::chrono::steady_clock clock;
  chrono::steady_clock::time_point begin;
  chrono::steady_clock::time_point end;
  double time;
  begin = clock.now();

  //search_exhaustive(global_queens, 0);
  descent_it(global_queens);

  end = clock.now();

  //time = (double) (end - begin) / CLOCKS_PER_SEC;
  
  cout << "\n\n\n";
  printBoard(global_queens);
  cout << "Final board with cost=" << calculateCost(global_queens) << ":\n";
  cout << "Runtime: " << getRunTimeString(begin, end) << endl;

  delete[] global_queens;
}

#include <omp.h>
#include <cstdlib>
#include <iostream>

using namespace std;

int* queens;
int size = 8;

void printBoard() {
  for (int z = 0; z < size; z++) cout << queens[z] << " ";
  cout << endl;
}

void generateBoard() {
  omp_set_num_threads(size);
  cout << "Generating board of size=" << size << endl;
#pragma omp parallel
  {
    int i = omp_get_thread_num();
    queens[i] = i;
  }
}

/**
 * @return true if the specified queen doesn't have any conflict.
 */
bool checkQueen(int q, int* conflict) {
  omp_set_num_threads(size);
  bool ret = true;
#pragma omp parallel
  {
    int i = omp_get_thread_num();
    if (i != q) {
      if (queens[i] == queens[q]) {
	ret = false;
	*conflict = i;
      }

      int diff = abs(i - q);
      if (abs(queens[i] - queens[q]) == diff) {
	ret = false;
	*conflict = i;
      }
    
    }
  }
  return ret;
}

/**
 * @return true if the entire board is OK.
 */
bool checkBoard(int* conflicts) {
  for (int i = 0; i < size; i ++) {
    int c;
    if (!checkQueen(i, &c)) {
      conflicts[0] = i;
      conflicts[1] = c;
     return false;
    }
  }
  return true;
}

void shuffleBoard() {
  omp_set_num_threads(size);
#pragma omp parallel
  {
    int i = omp_get_thread_num();
    queens[i] = rand() % size;
  }
}

void shuffleQueen(int q) {
  queens[q] = rand() % size;
}

void swap(int q1, int q2) {
  int tmp = queens[q1];
  queens[q1] = queens[q2];
  queens[q2] = tmp;
}

int main(int argc, char** argv) {
  if (argc > 1) {
    size = atoi(argv[1]);
  }
  queens = new int[size];
  
  generateBoard();
  printBoard();

  clock_t begin, end;
  double time;
  begin = clock();

  int* conflicts = new int[2];
  while (!checkBoard(conflicts)) {
    cout << "Conflict! ==> " << conflicts[0] << " & " << conflicts[1];
    int diff = abs(queens[conflicts[0]] - queens[conflicts[1]]);
    if (diff > 1) {
      cout << "\tswap!";
      int r = rand() % size;
      while (r == conflicts[0]) r = rand() % size;
      swap(conflicts[0], r);
    } else {
      cout << "\tshuffle!";
      shuffleQueen(conflicts[0]);
    }
    cout << endl;
    printBoard();
  }
  end = clock();

  time = (double) (end - begin) / CLOCKS_PER_SEC;
  cout << "Final board: ";
  printBoard();
  cout << "Runtime: " << time << " s" << endl;

  delete[] conflicts;
  delete[] queens;
}

#include <omp.h>
#include <cstdlib>
#include <iostream>

using namespace std;

int* global_queens;
int size = 8;


void printBoard(int* queens) {
  for (int z = 0; z < size; z++) cout << queens[z] << " ";
  cout << endl;
}

void generateBoard(int* queens) {
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

int main(int argc, char** argv) {
  srand(1);
  if (argc > 1) {
    size = atoi(argv[1]);
  }
  global_queens = new int[size];
  generateBoard(global_queens);
  printBoard(global_queens);


  clock_t begin, end;
  double time;
  begin = clock();

  for (int i = 0; i < 10000; i++) {
    
  }
  end = clock();

  time = (double) (end - begin) / CLOCKS_PER_SEC;
  //cout << "Final board with cost=" << cost << ": ";
  printBoard(global_queens);
  cout << "Runtime: " << time << " s" << endl;

  delete[] global_queens;
}

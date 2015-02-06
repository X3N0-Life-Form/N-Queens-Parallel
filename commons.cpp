#include "commons.h"

#include <iostream>

using namespace std;

extern int size; // declared in the n-queens main source files

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

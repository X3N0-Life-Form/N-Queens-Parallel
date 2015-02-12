#ifndef COMMONS_H
#define COMMONS_H

#include <cstdint>
#include <cstdlib>

#define NUMBER_OF_THREADS 4

void printBoard(int* queens);
void generateBoard(int* queens);
int* copyBoard(int* queens);

bool checkQueen(int q, int* queens);
bool checkBoard(int* queens);
int64_t lineConflict(int iValue, int jValue);
int64_t diagConflict(int difference, int iValue, int jValue);
int getFirstConflict(int* queens);

int64_t calculateCost(int* queens, int* conflicts = NULL);
int64_t updated_cost(int * queens, int index1, int index2);

void swap(int q1, int q2, int* queens);

int64_t descent(int* queens);
int64_t descent_2(int* queens);


#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>
#include <string.h>

#define MAX_ROWS 100
#define MAX_COLUMNS 100
#define ROUNDS 50000

// initializes board with 0's and 1's randomly
void initializeRandomState(unsigned char board[MAX_ROWS+1][MAX_COLUMNS+1]) {
  // ghost cells are set to 0 by default
  for (unsigned i = 1; i <= MAX_ROWS; ++i)
    for (unsigned j = 1; j <= MAX_COLUMNS; ++j)
      board[i][j] = rand() % 2;
}

// prints current board's state
void printCurrentState(unsigned char board[MAX_ROWS+1][MAX_COLUMNS+1]) {
  // i=1 and j=1 so ghost cells are ignored
  for (unsigned i = 1; i <= MAX_ROWS; ++i) {
    for (unsigned j = 1; j <= MAX_COLUMNS; ++j) {
      board[i][j]?printf("⬜"):printf("  ");
    }
    printf("\n");
  }
}

// counts the number of live neighbors
unsigned char liveNeighbors(unsigned char board[MAX_ROWS+1][MAX_COLUMNS+1], unsigned r, unsigned c) {

  return board[r-1][c-1] + board[r-1][c] + board[r-1][c+1] + board[r][c-1] +
         board[r][c+1] + board[r+1][c-1] + board[r+1][c] + board[r+1][c+1];
}

// sets next state by following Conway's Game of Life rules
void setNextState(unsigned char board[MAX_ROWS+1][MAX_COLUMNS+1]) {
  unsigned char nextBoard[MAX_ROWS+1][MAX_COLUMNS+1] = {0};

  // i=1, j=1 so ghost cells are ignored
  #pragma omp parallel for collapse(2)
  for (unsigned i = 1; i <= MAX_ROWS; ++i) {
    for (unsigned j = 1; j <= MAX_COLUMNS; ++j) {
      unsigned char totalLiveNeighbors = liveNeighbors(board, i, j);

      // Any live cell with two or three live neighbors survives. 
      // Any dead cell with three live neighbors becomes a live cell.
      if ((board[i][j] && totalLiveNeighbors == 2) || totalLiveNeighbors == 3)
        nextBoard[i][j] = 1;
      // else does nothing because nextBoard was filled with dead cells
    }
  }

  //swap matrix board and nextBoard 
  memcpy(board, nextBoard, sizeof(nextBoard));
  
}

int main(int argc, char const *argv[])
{
  srand(time(NULL));
  unsigned char board[MAX_ROWS+1][MAX_COLUMNS+1] = {0};
  initializeRandomState(board);

  unsigned i = ROUNDS;
  while (i--) {
    if (argc>1 && strcmp(argv[1], "-p") == 0) {
	system("clear");
    	printCurrentState(board);
    }
    setNextState(board);
  }
  
  return 0;
}

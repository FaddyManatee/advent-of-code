#include <stdio.h>

#define N     139
#define PAPER '@'

int count_accessible(char grid[N][N]) {
  int accessible = 0;

  int row, col;
  for (row = 0; row < N; ++row) {
    for (col = 0; col < N; ++col) {
      int count = 0;

      // We only care about cells with rolls of paper.
      if (grid[row][col] != PAPER) continue;

      // Upper two corners.
      if (row - 1 >= 0 && col - 1 >= 0 && grid[row - 1][col - 1] == PAPER) count++;
      if (row - 1 >= 0 && col + 1 <  N && grid[row - 1][col + 1] == PAPER) count++;
     
      // Lower two corners.
      if (row + 1 <  N && col - 1 >= 0 && grid[row + 1][col - 1] == PAPER) count++;
      if (row + 1 <  N && col + 1 <  N && grid[row + 1][col + 1] == PAPER) count++;
      
      // Up.
      if (row - 1 >= 0 && grid[row - 1][col] == PAPER) count++;
      
      // Down.
      if (row + 1 <  N && grid[row + 1][col] == PAPER) count++;
      
      // Left.
      if (col - 1 >= 0 && grid[row][col - 1] == PAPER) count++;

      // Right.
      if (col + 1 <  N && grid[row][col + 1] == PAPER) count++;

      // Less than four neighbouring rolls of paper. This one is accessible!
      if (count < 4) accessible++;
    }
  }

  return accessible;
}

int main(int argc, char **argv) {
   if (argc != 2) {
    printf("Usage: %s <path-to-puzzle-input>\n", argv[0]);
    return 1;
  }

  // Load puzzle input (a grid of rolls of paper).
  FILE *input = fopen(argv[1], "r");

  // Did the file load successfully?
  if (input == NULL) {
    printf("Failed to load puzzle input! (file:%s)\n", argv[1]);
    return 1;
  }

  // 2D array for input grid.
  char grid[N][N];

  int row, col;
  for (row = 0; row < N; ++row) {
    for (col = 0; col < N;) {
      char c = fgetc(input);

      if (c == '\n') continue;

      grid[row][col++] = c;
    }
  }

  int count = count_accessible(grid);

  printf("There are %d rolls of paper accessible by forklift!\n", count);

  return 0;
}


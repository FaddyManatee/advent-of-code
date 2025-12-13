#include <stdio.h>

#define N     139
#define PAPER '@'

int count_accessible(char grid[N][N]) {
  int accessible = 0;

  int row, col;
  for (row = 0; row < N; ++row) {
    for (col = 0; col < N; ++col) {
      // We only care about cells with rolls of paper.
      if (grid[row][col] != PAPER) continue;

      int count = 0;

      int row_d, col_d;
      for (row_d = -1; row_d <= 1; ++row_d) {
        for (col_d = -1; col_d <= 1; ++col_d) {
          if (row_d == 0 && col_d == 0)
            continue;

          int r = row + row_d;
          int c = col + col_d;

          if (r < 0 || r >= N || c < 0 || c >= N)
            continue;

          if (grid[r][c] == PAPER)
            count++;
        }
      }

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


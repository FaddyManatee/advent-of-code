#include <stdio.h>

#define N     139
#define PAPER '@'
#define EMPTY '.'

typedef struct {
  int row;
  int col;
} Cell;

int is_accessible(char grid[N][N], int row, int col) {
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

  return count < 4;
}

int remove_paper(char grid[N][N]) {
  int count = 0;

  Cell queue[N * N];
  int  q_head = 0;
  int  q_tail = 0;

  int queued[N][N] = { 0 };

  // Create queue with initially accessible paper.
  int row, col;
  for (row = 0; row < N; ++row) {
    for (col = 0; col < N; ++col) {
      if (grid[row][col] == PAPER && is_accessible(grid, row, col)) {
        queue[q_tail++] = (Cell) { row, col };
        queued[row][col] = 1;
      }
    }
  }

  // Process queue.
  while (q_head < q_tail) {
    Cell cell = queue[q_head++];
  
    row = cell.row;
    col = cell.col;

    if (grid[row][col] != PAPER)
      continue;

    // Remove dequeued paper from the grid.
    grid[row][col] = EMPTY;
    count++;

    // Check and enqueue acessible paper neighbours.
    int row_d, col_d;
    for (row_d = -1; row_d <= 1; ++row_d) {
      for (col_d = -1; col_d <= 1; ++col_d) {
        if (row_d == 0 && col_d == 0)
          continue;

        int r = row + row_d;
        int c = col + col_d;

        if (r < 0 || r >= N || c < 0 || c >= N)
          continue;

        if (grid[r][c] == PAPER && !queued[r][c]) {
          if (is_accessible(grid, r, c)) {
            queue[q_tail++] = (Cell) { r, c };
            queued[r][c] = 1;
          }
        }
      }
    }
  }

  return count;
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

  int count = remove_paper(grid);

  printf("%d rolls of paper can be removed by the forklifts!\n", count);

  return 0;
}


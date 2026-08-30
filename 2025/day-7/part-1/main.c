#include <stdio.h>

#include "list.h"

enum Symbols {
  ENTRY = 'S',
  BEAM  = '|',
  SPLIT = '^',
  EMPTY = '.'
};

int valid_symbol(int c) {
  if (c == ENTRY || c == BEAM || c == SPLIT || c == EMPTY)
    return 1;
  return 0;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <path-to-puzzle-input>\n", argv[0]);
    return 1;
  }

  int return_code = 0;

  // Load puzzle input.
  FILE *input = fopen(argv[1], "r");

  // Did the file load successfully?
  if (input == NULL) {
    printf("Failed to load puzzle input! (file:%s)\n", argv[1]);
    return 1;
  }

  // The tachyon manifold.
  List *manifold = new_list();

  int c;
  
  int n_read = 0;
  int n_row  = 0;

  int new_row = 1;

  // Read initial state of the tachyon manifold (puzzle input).
  while ((c = fgetc(input)) != EOF) {
    if (c == '\n') {
      new_row = 1;
      continue;
    }

    if (!valid_symbol(c)) {
      printf("Bad character found in input file when processing symbols: '%c'\n", c);
      return_code = 1;
      goto cleanup;
    }

    if (new_row) {
      n_row++;
      new_row = 0;
    }

    n_read++;
    insert_end(manifold, c);
  }

  // Sanity check that the tachyon manifold shape is square.
  if (n_read % n_row) {
    printf("The tachyon manifold shape is not square!\n");
    return_code = 1;
    goto cleanup;
  }

  int n_col = n_read / n_row;

  int n_splits = 0;

  // Simulate the tachyon manifold.
  int row, col;
  for (row = 0; row < n_row; ++row) {
    for (col = 0; col < n_col; ++col) {
      char symbol = get_data_2d_list(manifold, row, col, n_col);

      if (symbol == BEAM || symbol == ENTRY) {
        char below = get_data_2d_list(manifold, row + 1, col, n_col);
          
        // If there is a splitter under the beam or entry point.
        if (below == SPLIT) {
          update_data_2d_list(manifold, row + 1, col + 1, n_col, BEAM);
          update_data_2d_list(manifold, row + 1, col - 1, n_col, BEAM);
          n_splits++;
          continue;
        }

        // Otherwise pass the beam down the manifold.
        update_data_2d_list(manifold, row + 1, col, n_col, BEAM);
      }
    }
  }

  printf("The tachyon beam is split a total of %d times!\n", n_splits);

cleanup:
  fclose(input);
  delete_list(manifold);

  return return_code;
}


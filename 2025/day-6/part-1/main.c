#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "list.h"

#define PLUS  '+'
#define TIMES '*'

int main(int argc, char **argv) {
  int return_code = 0;

  if (argc != 2) {
    printf("Usage: %s <path-to-puzzle-input>\n", argv[0]);
    return 1;
  }

  // Load puzzle input.
  FILE *input = fopen(argv[1], "r");

  // Did the file load successfully?
  if (input == NULL) {
    printf("Failed to load puzzle input! (file:%s)\n", argv[1]);
    return 1;
  }

  List *numbers = new_list();
  List *symbols = new_list();

  char buffer[BUFFER_SIZE];

  int c;
  int x = 0;

  // Read all numbers until a symbol is detected.
  while ((c = fgetc(input)) != EOF) {
    if ((isblank(c) || isspace(c)) && x == 0)
      continue;

    if (isdigit(c) && x == BUFFER_SIZE - 1) {
      printf("Only numbers up to %d digits are supported!\n", BUFFER_SIZE - 1);
      return_code = 1;
      goto cleanup;
    }

    if (isdigit(c)) {
      buffer[x++] = c;
      continue;
    }

    if ((isblank(c) || isspace(c)) && x > 0) {
      buffer[x] = '\0';
      insert_end(numbers, buffer);
      x = 0;
      continue;
    }

    if (c == PLUS) {
      buffer[0] = PLUS;
      buffer[1] = '\0';
      insert_end(symbols, buffer);
      break;
    }

    if (c == TIMES) {
      buffer[0] = TIMES;
      buffer[1] = '\0';
      insert_end(symbols, buffer);
      break;
    }

    printf("Bad character found in input file when processing numbers: '%c'\n", c);
    return_code = 1;
    goto cleanup;
  }

  if (list_size(numbers) == 0) {
    printf("No numbers were found in the input file!\n");
    return_code = 1;
    goto cleanup;
  }

  // Read all symbols.
  while ((c = fgetc(input)) != EOF) {
    if (isblank(c) || isspace(c))
      continue;

    if (c == PLUS) {
      buffer[0] = PLUS;
      buffer[1] = '\0';
      insert_end(symbols, buffer);
      continue;
    }

    if (c == TIMES) {
      buffer[0] = TIMES;
      buffer[1] = '\0';
      insert_end(symbols, buffer);
      continue;
    }
    
    if (c != PLUS || c != TIMES) {
      printf("Bad character found in input file when processing symbols: '%c'\n", c);
      return_code = 1;
      goto cleanup;
    }
  }

  if (list_size(symbols) == 0) {
    printf("No symbols were found in the input file!\n");
    return_code = 1;
    goto cleanup;
  }

  int n_numbers = list_size(numbers);
  int n_symbols = list_size(symbols);

  // How many numbers in each Cephalopod math problem?
  int n_row = n_numbers / n_symbols;
  
  // How many Cephalopod math problems?
  int n_col = n_symbols;

  uint64_t grand_total = 0;

  // For each column, perform the calculation.
  int row, col;
  for (col = 0; col < n_col; ++col) {
    uint64_t total = 0;

    for (row = 0; row < n_row; ++row) {
      char *data      = get_data_2d_list(numbers, row, col, n_col);
      char *operation = get_data_1d_list(symbols, col);

      int number = atoi(data);

      switch (operation[0]) {
        case PLUS:
          total += number;
          break;

        case TIMES: {
          if (total == 0) total += number;
          else            total *= number;
          break;
        }
      }
    }

    grand_total += total;
  }

  printf("The grand total is %lu!\n", grand_total);

cleanup:
  fclose(input);
  delete_list(numbers);
  delete_list(symbols);

  return return_code;
}


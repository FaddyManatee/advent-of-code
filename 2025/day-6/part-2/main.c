#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "list.h"

#define PLUS  '+'
#define TIMES '*'

#define BUFFER_SIZE 8

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

  int c;
  int prev_c;

  int n_digits = 0;

  // Read all numbers until a symbol is detected.
  while ((c = fgetc(input)) != EOF) {
    if (isblank(c) || isspace(c)) {
      n_digits = 0;
      insert_end(numbers, '\0');
      continue;
    }

    if (n_digits >= BUFFER_SIZE - 1) {
      printf("Only numbers up to %d digits are supported!\n", BUFFER_SIZE - 1);
      return_code = 1;
      goto cleanup;
    }

    if (isdigit(c)) {
      n_digits++;
      insert_end(numbers, c);
      continue;
    }

    if (c == PLUS) {
      prev_c = c;
      insert_end(symbols, c);
      break;
    }

    if (c == TIMES) {
      prev_c = c;
      insert_end(symbols, c);
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
    if (isblank(c) || isspace(c)) {
      insert_end(symbols, prev_c);
      continue;
    }

    if (c == PLUS) {
      prev_c = c;
      insert_end(symbols, c);
      continue;
    }

    if (c == TIMES) {
      prev_c = c;
      insert_end(symbols, c);
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

  char buffer[BUFFER_SIZE];

  // Solve each Cephalopod math problem.
  uint64_t total = 0;

  int row, col;
  for (col = 0; col < n_col; ++col) {
    char operation = get_data_1d_list(symbols, col);
    char data_prev = '\0';
    
    int n_blank = 0;
    int x       = 0;
    
    for (row = 0; row < n_row; ++row) {
      char data = get_data_2d_list(numbers, row, col, n_col);

      // Shorter numbers with white space on their right.
      if (data == '\0' && data_prev != '\0') {
        buffer[x] = '\0';
        break;
      }

      if (data == '\0') {
        n_blank++;
        continue;
      }

      buffer[x++] = data;
      data_prev   = data;

      if (row == n_row - 1)
        buffer[x] = '\0';
    }

    // Do nothing for empty columns.
    if (n_blank == n_row) {
      grand_total += total;
      total = 0;
      continue;
    }

    int number = atoi(buffer);

    switch (operation) {
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

  printf("The grand total is %lu!\n", grand_total);

cleanup:
  fclose(input);
  delete_list(numbers);
  delete_list(symbols);

  return return_code;
}


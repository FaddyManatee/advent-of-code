#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX_BATTERY 100
#define N_DIGITS     12

int char_to_int(char c) {
  return c - 48;
}

void print_optimal(int *optimal, int n) {
  int x;
  for (x = n - 1; x >= 0; --x)
    printf("%d ", optimal[x]);
  printf("\n");
}

void find_optimal(char *bank, int *optimal, int n) {
  // Base case.
  if (n <= 0) return;

  int n_battery = strlen(bank);

  int max_power = 0;

  /* Find the largest power batteries that have at 
   * least n-1 batteries to their right.
   */
  int x;
  for (x = 0; x < n_battery; ++x) {
    int power = char_to_int(bank[x]);

    if (x + n > n_battery) continue;

    if (power > max_power) max_power = power;
  }

  /* We know the largest power battery in this (sub)-bank
   * for digit n. If this is larger than the existing
   * value for digit n, replace it.
   */ 
  if (optimal[n - 1] < max_power)
    optimal[n - 1] = max_power;

  /* For each battery with the largest power, find the 
   * next such batteries with at least n-2 batteries to
   * their right.
   */ 
  for (x = 0; x < n_battery; ++x) {
    int power = char_to_int(bank[x]);

    if (power == max_power) {
      char *sub_bank = bank + x + 1;

      // TODO: Is it possible to reduce the number of calls?
      find_optimal(sub_bank, optimal, n - 1);
    }
  }
}

uint64_t turn_on(char *bank, int n) {
  uint64_t joltage = 0;

  int x;
  int optimal[n];
  for (x = 0; x < n; ++x) optimal[x] = 0;
  
  find_optimal(bank, optimal, n);

  //print_optimal(optimal, n);

  // Interpret optimal digits as a single integer.
  int y = n - 1;
  for (int x = n - 1; x >= 0; --x)
    joltage = (joltage * 10) + optimal[x];

  return joltage;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <path-to-puzzle-input>\n", argv[0]);
    return 1;
  }

  // Load puzzle input (joltages from multiple banks of batteries).
  FILE *input = fopen(argv[1], "r");

  // Did the file load successfully?
  if (input == NULL) {
    printf("Failed to load puzzle input! (file:%s)\n", argv[1]);
    return 1;
  }

  // Sum of maximum joltage from each bank.
  uint64_t sum = 0;

  // There are 100 batteries in a bank (+1 null terminator).
  char bank[MAX_BATTERY + 1];

  int c;
  int x = 0;
  while ((c = fgetc(input)) != EOF) {
    if (c == '\n') continue;

    bank[x++] = c;

    if (x == MAX_BATTERY) {
      bank[x] = '\0';
      //printf("%s\n", bank);
      sum += turn_on(bank, N_DIGITS);
      //printf("\n");
      x = 0;
    }
  }

  fclose(input);

  printf("The total output joltage is %lu!\n", sum);

  return 0;
}


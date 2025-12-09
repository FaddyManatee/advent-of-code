#include <ctype.h>
#include <stdio.h>

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
  int sum = 0;

  // Battery digits. 'bat_1' is 10s and 'bat_2' is 1s.
  int bat_1 = 0;
  int bat_2 = 0;

  // Read and find maximum joltage possible from each bank.
  while (!feof(input)) {
    char c = fgetc(input);

    if (c == EOF) break;

    if (c == '\n') {
      int joltage = (bat_1 * 10) + bat_2;

      sum += joltage;
      bat_1 = bat_2 = 0;

      continue;
    }

    if (!isdigit(c)) {
      printf("Bad character in input file! (%c)\n", c);
      fclose(input);
      return 1;
    }

    int power = c - 48;

    if (power > bat_1) {
      // 'Peek' the next character in the file stream.
      char next = getc(input);

      if (next == '\n' || next == EOF) {
      // Digit is the last in the bank. Cannot be assigned to 'bat_1'.
        bat_2 = power;
      }
      else {
        bat_1 = power;
        bat_2 = 0;
      }

      // 'Unpeek'.
      ungetc(next, input);
    }
    else if (power > bat_2) {
      bat_2 = power;
    }
  }

  fclose(input);

  // Print total output joltage.
  printf("The total output joltage is %d!\n", sum);

  return 0;
}


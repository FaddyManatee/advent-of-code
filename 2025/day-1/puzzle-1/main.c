#include <stdio.h>

int main() {
  // Load puzzle input (a rotation sequence).
  FILE *input = fopen("input.txt", "r");

  // Did the file load successfully?
  if (input == NULL) {
    printf("Failed to load puzzle input!\n");
    return 1;
  }

  /*
   * A safe has a dial 0..99. The dial starts at 50.
   */
  int dial = 50;

  // The direction in which the dial is turned (L or R).
  char direction;

  // How much the dial is moved by.
  int magnitude;

  // Read and apply each rotation line from the puzzle input.
  int line = 1;

  while (!feof(input)) {
    int count = fscanf(input, "%c%d\n", &direction, &magnitude);

    // Both a direction and magnitude must be read.
    if (count != 2) {
      printf("Missing direction or magnitude on line %d!\n", line);
      fclose(input);
      return 1;
    }

    printf("%c", direction);
    printf(" with %d\n", magnitude);

    line++;
  }

  fclose(input);

  // Return the value of dial after applying all rotations.
  return dial;
}


#include <stdio.h>

#define LEFT  'L'
#define RIGHT 'R'

void apply_rotation(int rotation, int *dial) {
  // Dial has values 0..99 with wrap-around. Analogous to modulo 100.
  *dial = (*dial + rotation) % 100;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <path-to-puzzle-input>\n", argv[0]);
    return 1;
  }

  // Load puzzle input (a rotation sequence).
  FILE *input = fopen(argv[1], "r");

  // Did the file load successfully?
  if (input == NULL) {
    printf("Failed to load puzzle input! (file:%s)\n", argv[1]);
    return 1;
  }

  // The door has a dial 0..99. The dial starts at 50.
  int dial = 50;

  // The direction in which the dial is turned.
  char direction;

  // How much the dial is moved by.
  int magnitude;

  // The password is the number of times the dial is at 0.
  int password = 0;

  // Current line or rotation.
  int line = 1;

  // Read and apply each rotation from the puzzle input.
  while (!feof(input)) {
    int count = fscanf(input, "%c%d\n", &direction, &magnitude);

    // Both a direction and magnitude must be read.
    if (count != 2) {
      printf("Missing direction or magnitude! (line:%d)\n", line);
      fclose(input);
      return 1;
    }

    // Only accepted directions are 'L' and 'R'.
    if (direction != LEFT && direction != RIGHT) {
      printf("A direction must be either '%c' or '%c'! (line:%d)\n", LEFT, RIGHT, line);
      fclose(input);
      return 1;
    }

    switch (direction) {
      // Negative.
      case LEFT:
        apply_rotation(-magnitude, &dial);
        break;

      // Positive.
      case RIGHT:
        apply_rotation(magnitude, &dial);
        break;
    }

    // Does the dial point at 0?
    if (dial == 0)
      password++;

    line++;
  }

  fclose(input);

  // Print the password after applying all rotations.
  printf("The password to open the door is %d!\n", password);

  return 0;
}


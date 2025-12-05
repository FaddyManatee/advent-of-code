#include <stdio.h>
#include <stdlib.h>

#define LEFT  'L'
#define RIGHT 'R'

#define MAX_DIAL 99
#define MIN_DIAL 0

int apply_rotation(int rotation, int *dial) {
  // The number of times the dial lands on 0.
  int count = 0;

  // Left rotation.
  if (rotation < 0) {
    while (rotation != 0) {
      *dial = *dial - 1;

      // Is the dial now at 0?
      if (*dial == MIN_DIAL)
        count++;

      // Wrap-around.
      if (*dial < MIN_DIAL)
        *dial = MAX_DIAL;
    
      rotation++;
    }
  }
  // Right rotation.
  else if (rotation > 0) {
    while (rotation != 0) {
      *dial = *dial + 1;

      // Wrap-around.
      if (*dial > MAX_DIAL)
        *dial = MIN_DIAL;

      // Is the dial now at 0?
      if (*dial == MIN_DIAL)
        count++;

      rotation--;
    }
  }

  return count;
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
        password += apply_rotation(-magnitude, &dial);
        break;

      // Positive.
      case RIGHT:
        password += apply_rotation(magnitude, &dial);
        break;
    }

    line++;
  }

  fclose(input);

  // Print the password after applying all rotations.
  printf("The password to open the door is %d!\n", password);

  return 0;
}


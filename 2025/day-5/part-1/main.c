#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define N 256

int is_fresh(uint64_t id, uint64_t *lower, uint64_t *upper, int n_intervals) {
  int count = 0;

  int x;
  for (x = 0; x < n_intervals; ++x) {
    if (id >= lower[x] && id <= upper[x]) 
      count++;
  }

  if (count > 0) return 1;
  else           return 0;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <path-to-puzzle-input>\n", argv[0]);
    return 1;
  }

  // Load puzzle input (a sequence of ID intervals and IDs).
  FILE *input = fopen(argv[1], "r");

  // Did the file load successfully?
  if (input == NULL) {
    printf("Failed to load puzzle input! (file:%s)\n", argv[1]);
    return 1;
  }

  // Lower bounds.
  uint64_t lower[N];
  
  // Upper bounds.
  uint64_t upper[N];

  // Number of fresh ingredient IDs.
  int count = 0;

  /* Read one interval from the file at a time and update bounds.
   * We are creating a single, combined interval from multiple intervals.
   */
  int x = 0;
  char line[64];
  while (fgets(line, sizeof(line), input)) {
    uint64_t l;
    uint64_t u;

    // An empty line signals the start of the next section, so break.
    if (strcmp(line, "\n") == 0) break;

    if (x == N) {
      printf("Too many intervals!\n");
      fclose(input);
      return 1;
    }

    if (sscanf(line, "%lu-%lu", &l, &u) != 2) {
      printf("Bad format!\n");
      fclose(input);
      return 1;
    }

    if (l > u) {
      printf("Invalid interval! (%lu-%lu)\n", l, u);
      fclose(input);
      return 1;
    }

    lower[x] = l;
    upper[x] = u;
    x++;
  }

  // Count IDs within this aggregate interval (count fresh ingredients).
  uint64_t id;
  while (fgets(line, sizeof line, input)) {
    if (sscanf(line, "%lu", &id) != 1) {
      printf("Bad format!\n");
      fclose(input);
      return 1;
    }

    if (is_fresh(id, lower, upper, x)) {
      printf("%lu is fresh!\n", id);
      count++;
    }
    else {
      printf("%lu is spoiled!\n", id);
    }
  }
  printf("\n");

  fclose(input);

  // Print the result.
  printf("The number of available ingredient IDs that are fresh is %u!\n", count);

  return 0;
}


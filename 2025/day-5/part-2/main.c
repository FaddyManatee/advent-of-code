#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "list.h"


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

  // Linked list of intervals.
  List *list = new_list();

  char line[64];
  while (fgets(line, sizeof(line), input)) {
    uint64_t lower;
    uint64_t upper;

    // An empty line signals the start of the next (now irrelevant) section, so break.
    if (strcmp(line, "\n") == 0) break;

    if (sscanf(line, "%lu-%lu", &lower, &upper) != 2) {
      printf("Bad format!\n");
      fclose(input);
      delete_list(list);
      return 1;
    }

    if (lower > upper) {
      printf("Invalid interval! (%lu-%lu)\n", lower, upper);
      fclose(input);
      delete_list(list);
      return 1;
    }

    Interval interval = { lower, upper };

    insert_end(list, interval);
  }

  fclose(input);

  // Merge overlapping intervals.
  merge_intervals(list);

  uint64_t count = 0;

  printf("Merged intervals:\n");

  // Count IDs in each of the non-overlapping intervals.
  int x;
  for (x = 0; x < list_size(list); ++x) {
    Interval merged = get_interval(list, x);

    printf("%lu-%lu\n", merged.lower, merged.upper);

    count += merged.upper - merged.lower + 1;
  }

  // Print the result.
  printf("\nThe number of available ingredient IDs that are fresh is %lu!\n", count);

  delete_list(list);

  return 0;
}


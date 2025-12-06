#include <stdio.h>

int count_digits(long n) {
  if (n == 0) return 1;

  int count = 0;

  while (n != 0) {
    n /= 10;
    count++;
  }

  return count;
}

long same_length_bounds(long lower, long upper, int n_digits) {
  long sum = 0;

  // Which integers split n_digits into groups of equal size?
  int n_groups;
  for (n_groups = n_digits; n_groups >= 2; --n_groups) {
    if (n_digits % n_groups != 0) continue;
    
    int size = n_digits / n_groups;
    
    printf("%d -> %d groups of size %d\n", n_digits, n_groups, size);
  }

  return sum;
} 

long sum_invalid_ids(long lower, long upper) {
  // Get number of digits in each bound.
  int n_digits_lower = count_digits(lower);
  int n_digits_upper = count_digits(upper);

  if (n_digits_lower == n_digits_upper)
    return same_length_bounds(lower, upper, n_digits_upper);

  else
    return 0;

}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <path-to-puzzle-input>\n", argv[0]);
    return 1;
  }

  // Load puzzle input (a sequence of ID ranges).
  FILE *input = fopen(argv[1], "r");

  // Did the file load successfully?
  if (input == NULL) {
    printf("Failed to load puzzle input! (file:%s)\n", argv[1]);
    return 1;
  }

  // Lower bound.
  long lower;
  
  // Upper bound.
  long upper;

  // Sum of all invalid IDs.
  long sum = 0;

  // Read one range from the file at a time and process it.
  while (!feof(input)) {
    int count = fscanf(input, "%ld-%ld", &lower, &upper);

    // Both a lower and upper bound must be read.
    if (count != 2) {
      printf("Bad format!\n");
      fclose(input);
      return 1;
    }

    // Consume the comma delimiter or new line.
    char c = fgetc(input);
    if (c == '\n') break;

    printf("Range: %ld-%ld\n", lower, upper);

    sum += sum_invalid_ids(lower, upper);

    break;
    //printf("\n\n");
  }

  fclose(input);

  // Print the result.
  printf("The sum of all invalid IDs equals %ld!\n", sum);

  return 0;
}


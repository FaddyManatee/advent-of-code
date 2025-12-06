#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_digits(int64_t n) {
  if (n == 0) return 1;

  int count = 0;

  while (n != 0) {
    n /= 10;
    count++;
  }

  return count;
}

int64_t same_length_bounds(int64_t lower, int64_t upper, int n_digits) {
  int64_t sum = 0;

  // Which integers split n_digits into groups of equal size?
  int n_groups;
  for (n_groups = n_digits; n_groups >= 2; --n_groups) {
    if (n_digits % n_groups != 0) continue;
    
    int size = n_digits / n_groups;

    // Get string representations of interval bounds.
    char lower_string[n_digits + 1];
    char upper_string[n_digits + 1];

    snprintf(lower_string, sizeof(lower_string), "%ld", lower);
    snprintf(upper_string, sizeof(upper_string), "%ld", upper);

    // The first group is the repeating pattern (first 'size' digits). 
    char    pattern_string[size + 1];
    int64_t pattern_value;

    strncpy(pattern_string, lower_string, size);
    pattern_string[size] = '\0';
    pattern_value = strtoll(pattern_string, NULL, 10);

    printf("%d -> %d groups of size %d (%ld)\n", n_digits, n_groups, size, pattern_value);

    // Generate a candidate ID (pattern repeated n_groups times).
    char    candidate_string[n_digits + 1];
    int64_t candidate_value;

    int x;
    strcpy(candidate_string, pattern_string);
    for (x = 0; x < n_groups - 1; ++x)
      strcat(candidate_string, pattern_string);
    candidate_string[n_digits] = '\0';
    candidate_value = strtoll(candidate_string, NULL, 10);

    // If the candidate ID is within the interval, it is invalid!
    if (candidate_value >= lower && candidate_value <= upper) {
      printf("Accepted: %ld\n", candidate_value);
      sum += candidate_value;
    }
    else
      printf("Rejected: %ld\n", candidate_value);
  }

  return sum;
}

int64_t sum_invalid_ids(int64_t lower, int64_t upper) {
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

  // Load puzzle input (a sequence of ID intervals).
  FILE *input = fopen(argv[1], "r");

  // Did the file load successfully?
  if (input == NULL) {
    printf("Failed to load puzzle input! (file:%s)\n", argv[1]);
    return 1;
  }

  // Lower bound.
  int64_t lower;
  
  // Upper bound.
  int64_t upper;

  // Sum of all invalid IDs.
  int64_t sum = 0;

  // Read one interval from the file at a time and process it.
  while (!feof(input)) {
    int count = fscanf(input, "%ld-%ld", &lower, &upper);

    // Both a lower and upper bound must be read.
    if (count != 2) {
      printf("Bad format!\n");
      fclose(input);
      return 1;
    }

    if (lower > upper) {
      printf("Invalid interval! (%ld-%ld)\n", lower, upper);
      fclose(input);
      return 1;
    }

    // Consume the comma delimiter or new line character.
    char c = fgetc(input);
    if (c == '\n') break;

    printf("Interval: %ld-%ld\n", lower, upper);

    sum += sum_invalid_ids(lower, upper);

    break;
  }

  fclose(input);

  // Print the result.
  printf("The sum of all invalid IDs equals %ld!\n", sum);

  return 0;
}


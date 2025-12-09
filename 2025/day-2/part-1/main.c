#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int count_digits(uint64_t n) {
  if (n == 0) return 1;

  int count = 0;

  while (n != 0) {
    n /= 10;
    count++;
  }

  return count;
}

int is_same_char_string(char *s) {
  char first = s[0];

  int x;
  for (x = 1; x < strlen(s); ++x)
    if (s[x] != first) return 0;

  return 1;
}

int special_check(char *s) {
  char a = s[0];
  char b = s[1];

  int even = 1;

  int x;
  for (x = 0; x < strlen(s); ++x) {
    if (even)
      if (s[x] != a) return 0; else even = 0;
    else
      if (s[x] != b) return 0; else even = 1; 
  }

  return 1;
}

uint64_t same_length_bounds(uint64_t lower, uint64_t upper) {
  uint64_t sum = 0;

  int n_digits = count_digits(upper);

  if (n_digits % 2 != 0) {
    printf("  Odd digits in interval. Skipping!\n\n");
    return sum;
  }

  int n_groups;
  for (n_groups = n_digits; n_groups >= 2; --n_groups) {
    // Which integers split n_digits into groups of equal size?
    if (n_digits % n_groups != 0) continue;
    
    int size = n_digits / n_groups;

    // Skip odd number of groups.
    if (n_groups % 2 != 0) continue;

    // Get string representations of interval bounds.
    char lower_string[n_digits + 1];
    char upper_string[n_digits + 1];

    snprintf(lower_string, sizeof(lower_string), "%ld", lower);
    snprintf(upper_string, sizeof(upper_string), "%ld", upper);

    // The first group is the repeating pattern (first 'size' digits). 
    char     pattern_string[size + 1];
    uint64_t pattern_value;

    strncpy(pattern_string, lower_string, size);
    pattern_string[size] = '\0';
    pattern_value = strtoll(pattern_string, NULL, 10);

    printf("  %d groups of size %d\n", n_groups, size);

    int first = 1;

    do {
      if (!first)
        snprintf(pattern_string, sizeof(pattern_string), "%ld", pattern_value);
      else
        first = 0;

      // Generate a candidate ID (the pattern repeated n_groups times).
      char     candidate_string[n_digits + 1];
      uint64_t candidate_value;

      int x;
      strcpy(candidate_string, pattern_string);
      for (x = 0; x < n_groups - 1; ++x)
        strcat(candidate_string, pattern_string);
      candidate_string[n_digits] = '\0';
      candidate_value = strtoll(candidate_string, NULL, 10);

      // The candidate violates the upper bound. This pattern size is exhausted.
      if (candidate_value > upper) {
        printf("    Rejected: %ld\n\n", candidate_value);
        break;
      }
      // The candidate violates the lower bound.
      else if (candidate_value < lower) {
        printf("    Rejected: %ld\n", candidate_value);
        
        if   (count_digits(pattern_value + 1) > count_digits(pattern_value)) break;
        else pattern_value++;
      }
      // The candidate is within the interval, so it is invalid!
      else {
        // First check that the candidate has not already been accepted.
        if (size != 1 && is_same_char_string(candidate_string)) {
          printf("    Rejected: %ld (duplicate)\n", candidate_value);
          
          if   (count_digits(pattern_value + 1) > count_digits(pattern_value)) break;
          else pattern_value++;

          continue;
        }

        if (n_groups == 2 && size > 2 && special_check(candidate_string)) {
           printf("    Rejected: %ld (duplicate)\n", candidate_value);
          
          if   (count_digits(pattern_value + 1) > count_digits(pattern_value)) break;
          else pattern_value++;

          continue; 
        }

        printf("    Accepted: %ld\n", candidate_value);
        sum += candidate_value;

        if   (count_digits(pattern_value + 1) > count_digits(pattern_value)) break;
        else pattern_value++;
      }
    } while (1);
  }

  return sum;
}

uint64_t different_length_bounds(uint64_t lower, uint64_t upper) {
  uint64_t sum = 0;

  /* 
   * Break down the problem to reuse same_length_bounds()
   *
   * The interval 0-9999 can be decomposed into 4 non-overlapping intervals:
   *
   * 1) 0-9
   * 2) 10-99
   * 3) 100-999
   * 4) 1000-9999
   */

  // Get number of digits in each bound.
  int n_digits_lower = count_digits(lower);
  int n_digits_upper = count_digits(upper);

  uint64_t l = 0;
  uint64_t u = 9;

  int x;
  for (x = 1; x <= n_digits_upper; ++x) {
    int n_digits_l = count_digits(l);

    if (n_digits_l < n_digits_lower) {
      ;
    }
    else if (n_digits_l == n_digits_lower) {
      printf("Sub-interval: %ld-%ld\n", lower, u);
      sum += same_length_bounds(lower, u);
    }
    else if (n_digits_l == n_digits_upper) {
      printf("Sub-interval: %ld-%ld\n", l, upper);
      sum += same_length_bounds(l, upper);
    }
    else {
      printf("Sub-interval: %ld-%ld\n", l, u);
      sum += same_length_bounds(l, u);
    }

    l = pow(10, x);
    u = pow(10, (x + 1)) - 1;
  }

  return sum;
}

uint64_t sum_invalid_ids(uint64_t lower, uint64_t upper) {
  // Get number of digits in each bound.
  int n_digits_lower = count_digits(lower);
  int n_digits_upper = count_digits(upper);

  printf("Interval: %ld-%ld\n", lower, upper);

  if (n_digits_lower == n_digits_upper)
    return same_length_bounds(lower, upper);
  else
    return different_length_bounds(lower, upper);
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
  uint64_t lower;
  
  // Upper bound.
  uint64_t upper;

  // Sum of all invalid IDs.
  uint64_t sum = 0;

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

    sum += sum_invalid_ids(lower, upper);

    if (c == '\n') break;
  }

  fclose(input);

  // Print the result.
  printf("The sum of all invalid IDs equals %ld!\n", sum);

  return 0;
}


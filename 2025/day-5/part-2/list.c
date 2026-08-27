#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "list.h"

struct Item {
  Item    *next;
  Interval interval;
};

struct List {
  Item *start;
  Item *end;
  int   size;
};

List *new_list() {
  List *list = (List *) malloc(sizeof(List));

  list->start = NULL;
  list->end   = NULL;
  list->size  = 0;

  return list;
}

Interval get_interval(List *list, int index) {
  if (is_empty(list) || index >= list->size) 
    return (Interval) { 0, 0 };

  Item *pointer = list->start;

  int i = 0;
  while (pointer != NULL) {
    if (i++ == index) 
      return pointer->interval;
    pointer = pointer->next;
  }

  return (Interval) { 0, 0 };
}

void remove_interval(List *list, Interval interval) {
  if (is_empty(list))
    return;

  Item *pointer = list->start;
  Item *prev    = NULL;

  int i = 0;
  while (pointer != NULL) {
    Interval current = pointer->interval;

    if (current.lower != interval.lower && current.upper != interval.upper) {
      prev    = pointer;
      pointer = pointer->next;
      i++;
      continue;
    }
    
    // Removing the first list item.
    if (i == 0) {
      list->start = pointer->next;
      list->size  = list->size - 1;

      free(pointer);

      return;
    }

    // Removing other list items.
    prev->next = pointer->next;

    if (pointer == list->end)
      list->end = prev;
    
    free(pointer);
    
    break;
  }

  list->size = list->size - 1;
}

void merge_intervals(List *list) {
  if (list_size(list) == 1) return;

  int n_merged;

  do {
    n_merged = 0;

    int x, y;
    for (x = 0; x < list_size(list); ++x) {
      for (y = 0; y < list_size(list); ++y) {
        if (x == y) continue;

        Interval ix = get_interval(list, x);
        Interval iy = get_interval(list, y);

        // ix should be the interval with the smallest lower bound. 
        if (iy.lower < ix.lower) {
          Interval temp = ix;
          ix = iy;
          iy = temp;
        }

        Interval merged_interval;
        int      merge_possible = 0;

        // Interval lies within another interval.
        if (iy.lower >= ix.lower && iy.lower <= ix.upper && iy.upper <= ix.upper) {
          merge_possible = 1;

          merged_interval.lower = ix.lower;
          merged_interval.upper = ix.upper;
        }
        // Interval overlaps another interval.
        else if (iy.lower >= ix.lower && iy.lower <= ix.upper && iy.upper >= ix.upper) {
          merge_possible = 1;

          merged_interval.lower = ix.lower;
          merged_interval.upper = iy.upper;
        }

        if (merge_possible) {
          printf(
            "Merged %lu-%lu and %lu-%lu -> %lu-%lu\n",
            ix.lower, ix.upper, 
            iy.lower, iy.upper,
            merged_interval.lower, merged_interval.upper
          );

          n_merged++;

          remove_interval(list, ix);
          remove_interval(list, iy);
          insert_end(list, merged_interval);
          print_list(list);
          printf("\n");
        }
      }
    }
  } while (n_merged > 0);
}

void print_list(List *list) {
  Item *pointer = list->start;

  while (pointer != NULL) {
    Interval interval = pointer->interval;

    if (pointer->next == NULL) {
      printf("%lu-%lu (%d items)\n", interval.lower, interval.upper, list->size);
      break;
    }
    else
      printf("%lu-%lu, ", interval.lower, interval.upper);

    pointer = pointer->next;
  }
}

void delete_list(List *list) {
  Item *pointer = list->start;

  Item *next;
  while (pointer != NULL) {
    next = pointer->next;
    free(pointer);
    pointer = next;
  }

  free(list);
}

void insert_end(List *list, Interval interval) {
  Item *new = (Item *) malloc(sizeof(Item));

  new->interval = interval;
  new->next     = NULL;

  // Insert to an empty list.
  if (is_empty(list)) {
    list->start = new;
    list->end   = new;
    list->size  = 1;
  }
  // Insert to a non-empty list.
  else {
    list->end->next = new;
    list->end       = new;
    list->size      = list->size + 1;
  }
}

int list_size(List *list) {
  return list->size;
} 

int is_empty(List *list) {
  return list->size == 0;
}


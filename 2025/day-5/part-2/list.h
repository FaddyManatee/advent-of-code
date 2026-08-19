#ifndef LIST_H
#define LIST_H

#include <stdint.h>

typedef struct Interval {
  uint64_t lower;
  uint64_t upper;
} Interval;

typedef struct List List;
typedef struct Item Item;

List     *new_list();
Interval  get_interval(List *list, int index);
void      insert_end(List *list, Interval interval);
void      remove_interval(List *list, Interval interval);
void      merge_intervals(List *list);
void      print_list(List *list);
void      delete_list(List *list);
int       is_empty(List *list);
int       list_size(List *list);

#endif  // LIST_H


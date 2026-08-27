#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

struct Item {
  Item *next;
  char  data;
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

char get_data_1d_list(List *list, int index) {
  if (is_empty(list) || index >= list->size) 
    return '\0';

  Item *pointer = list->start;

  int i = 0;
  while (pointer != NULL) {
    if (i++ == index) 
      return pointer->data;
    pointer = pointer->next;
  }

  return '\0';
}

char get_data_2d_list(List *list, int row, int col, int n_col) {
  // 2-dimensional to 1-dimensional mapping.
  int index = (row * n_col) + col;

  return get_data_1d_list(list, index);
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

void insert_end(List *list, char data) {
  Item *new = (Item *) malloc(sizeof(Item));

  if (new == NULL) {
    free(new);
    return;
  }

  new->data = data;
  new->next = NULL;

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


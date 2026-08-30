#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

#define BUCKET_SIZE 128

struct Item {
  Item *next;
  char  data[BUCKET_SIZE];
  int   index;
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

  int list_index   = index / BUCKET_SIZE;
  int bucket_index = index % BUCKET_SIZE;

  Item *pointer = list->start;

  int i = 0;
  while (pointer != NULL) {
    if (i++ == list_index) 
      return pointer->data[bucket_index];
    pointer = pointer->next;
  }

  return '\0';
}

char get_data_2d_list(List *list, int row, int col, int n_col) {
  // 2-dimensional to 1-dimensional mapping.
  int index = (row * n_col) + col;

  return get_data_1d_list(list, index);
}

void update_data_1d_list(List *list, int index, char data) {
  if (is_empty(list) || index >= list->size) 
    return;

  int list_index   = index / BUCKET_SIZE;
  int bucket_index = index % BUCKET_SIZE;

  Item *pointer = list->start;

  int i = 0;
  while (pointer != NULL) {
    if (i++ == list_index && pointer->index >= bucket_index) 
      pointer->data[bucket_index] = data;
    pointer = pointer->next;
  }
}

void update_data_2d_list(List *list, int row, int col, int n_col, char data) {
  // 2-dimensional to 1-dimensional mapping.
  int index = (row * n_col) + col;

  update_data_1d_list(list, index, data);
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
  // Insert to next free bucket space.
  if (!is_empty(list) && list->end->index <= BUCKET_SIZE - 1) {   
    list->end->data[list->end->index++] = data;
    list->size++;
    return;
  }

  Item *new = (Item *) malloc(sizeof(Item));
  
  if (new == NULL)
    return;
  new->index = 0;

  new->data[new->index++] = data;
  new->next               = NULL;

  // Insert to an empty list.
  if (is_empty(list)) {
    list->start = new;
    list->end   = new;
    list->size  = 1;

    return;
  }

  // No available space.
  list->end->next = new;
  list->end       = new;
  list->size      = list->size + 1;
}

int list_size(List *list) {
  return list->size;
} 

int is_empty(List *list) {
  return list->size == 0;
}


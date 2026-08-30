#ifndef LIST_H
#define LIST_H

#include <stdint.h>

typedef struct List List;
typedef struct Item Item;

List *new_list();
char  get_data_1d_list(List *list, int index);
char  get_data_2d_list(List *list, int row, int col, int n_col);
void  update_data_1d_list(List *list, int index, char data);
void  update_data_2d_list(List *list, int row, int col, int n_col, char data);
void  insert_end(List *list, char data);
void  delete_list(List *list);
int   is_empty(List *list);
int   list_size(List *list);

#endif  // LIST_H


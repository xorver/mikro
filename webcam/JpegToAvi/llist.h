
#ifndef _LLIST_H_
#define _LLIST_H_


typedef struct _List List;


struct _List
{
  void *data;
  List *next;
  List *prev;
};

List        *list_erase      (List *l);
List        *list_erased     (List *l);
void         list_rerase     (List *l);
void         list_rerased    (List *l);
void         list_insert     (List *l, void *data);
List        *list_push_front (List *l, void *data);
List        *list_push_back  (List *l, void *data);
void         list_pop_front  (List *l);
void         list_pop_back   (List *l);
void         list_popd_front (List *l);
void         list_popd_back  (List *l);
List        *list_front      (List *l);
List        *list_back       (List *l);

void         list_remove     (List *l, void *data);
List        *list_find       (List *l, void *data);
List        *list_nth        (List *l, unsigned int n);
unsigned int list_size       (List *l);



#endif


#include "llist.h"
#include <stdlib.h>

List *list_erase(List *l)
{
  List *next = l->next;

  if (l->prev)
    l->prev->next = next;
  if (next)
    l->next->prev = l->prev;

  free(l);

  return next;
}


List *list_erased(List *l)
{
  if (l->data) {
    free(l->data);
    l->data = 0;
  }
  return list_erase(l);
}

void list_rerase(List *l)
{
  if (l->next)
    list_rerase(l->next);
  l->next = 0;

  free(l);
}

void list_rerased(List *l)
{
  if (l->next)
    list_rerased(l->next);
  l->next = 0;

  if (l->data) {
    free(l->data);
    l->data = 0;
  }
  free(l);
}

void list_insert(List *l, void *data)
{
  List *next = (List *) malloc(sizeof(List));
  next->data = l->data;
  next->next = l->next;
  next->prev = l;
  l->data = data;
  l->next = next;
}

List *list_push_front(List *l, void *data)
{
  l = list_front(l);
  l->prev = (List *) malloc(sizeof(List));
  l->prev->data = data;
  l->prev->next = l;
  l->prev->prev = 0;

  return l->prev;
}

List *list_push_back(List *l, void *data)
{
  l = list_back(l);
  l->next = (List *) malloc(sizeof(List));
  l->next->data = data;
  l->next->next = 0;
  l->next->prev = l;

  return l->next;
}


void list_pop_front(List *l)
{
  list_erase(list_front(l));
}


void list_pop_back(List *l)
{
  list_erase(list_back(l));
}


void list_popd_front(List *l)
{
  list_erased(list_front(l));
}


void list_popd_back(List *l)
{
  list_erased(list_back(l));
}


List *list_front(List *l)
{
  while (l->prev)
    l = l->prev;
  return l;
}


List *list_back(List *l)
{
  while (l->next)
    l = l->next;
  return l;
}

void list_remove(List *l, void *data)
{
  List *l2;
  while ((l2 = list_find(l, data)))
    l = list_erase(l2); 
}

List *list_find(List *l, void *data)
{
  while (l && (l->data != data))
    l = l->next;
  return l;
}

List *list_nth(List *l, unsigned int n) 
{
  int i;
  for (i = 0; (i < n) && (l); ++i, l = l->next); 
  return l;
}

unsigned int list_size(List *l)
{
  int i;
  for (i = 0; (l); ++i, l = l->next); 
  return i;
}


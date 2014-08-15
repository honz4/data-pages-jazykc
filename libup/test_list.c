
#include "up/list.h"

#include <stdio.h>
#include <stdlib.h>


typedef struct Ldata {
  union {
    int       i;
    long      l;
    short     h2[2];
    long long ll;
    short     h4[4];
    double    d;
    void     *p;
  } data;
  struct up_list list;
  struct up_list list2;/**< seznam Ldata polozek muze byt triden nekolik zpusoby. */
} Ldata;

/** pro Ldata si udelame nas alokator/recyklator */
static UP_DEFLIST(ldata_free_list);

Ldata *ldata_new() {
  Ldata *ldata;
  if (ldata = up_list_typepop(&ldata_free_list, Ldata, list)) {
     return ldata;
  }
  // neni co recyklovat, normalne malloc()ujeme
  ldata = malloc(sizeof *ldata); 
  // je potreba inicializovat struct up_list cleny?
  up_list_init(&(ldata->list));
  up_list_init(&(ldata->list2));
  return ldata;
}

/** nevolame free(), ale recyklujeme pro ldata_new()
 */
void ldata_free(Ldata *ldata) {
  up_list_prepend(&ldata_free_list, &(ldata->list));
}

int main(int argc, char *argv[])
{
  Ldata *datap;
  UP_DEFLIST(data_list);
  UP_DEFLIST(data_list1);
  UP_DEFLIST(data_list2);
  struct up_list *list1, *item1;
  int i;

  for (i=0; i< 7; i++) {
    datap = ldata_new();
    datap->data.d = 1.0/(i+1);
    up_list_prepend(&data_list, &(datap->list));
  }
  up_list_foreach(item1, &data_list) {
    datap = container_of(item1, Ldata,list);
    printf("data.d = %g, ", datap->data.d);
  } putchar('\n');


  for (i=0; i< 10; i++) {
    datap = ldata_new();
    datap->data.i = i;
    list1 = up_list_prepend(&data_list1, &(datap->list));
  }
    datap = ldata_new();
    datap->data.i = i;
    up_list_prepend(up_list_last(&data_list1)->prev, &(datap->list));

  up_list_foreach(item1, &data_list1) {
    datap = container_of(item1, Ldata,list);
    printf("data.i = %d, ", datap->data.i);
  } putchar('\n');

  up_list_foreach_reverse(item1, &data_list1) {
    datap = container_of(item1, Ldata,list);
    printf("data.i = %d, ", datap->data.i);
  } putchar('\n');

  return EXIT_SUCCESS;
}

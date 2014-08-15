
#include "up/value.h"

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
  up_value *datap;
  UP_DEFLIST(data_list);
  UP_DEFLIST(data_list1);
  UP_DEFLIST(data_list2);
  struct up_list *list1, *item1;
  int i;

  for (i=0; i< 7; i++) {
    datap = up_value_new();
    datap->value.d = 1.0/(i+1);
    up_list_prepend(&data_list, &(datap->list));
  }
  up_list_foreach(item1, &data_list) {
    datap = container_of(item1, up_value,list);
    printf("value.d = %g, ", datap->value.d);
  } putchar('\n');


  for (i=0; i< 10; i++) {
    datap = up_value_new();
    datap->value.i = i;
    list1 = up_list_prepend(&data_list1, &(datap->list));
  }
    datap = up_value_new();
    datap->value.i = i;
    up_list_prepend(up_list_last(&data_list1)->prev, &(datap->list));

  up_list_foreach(item1, &data_list1) {
    datap = container_of(item1, up_value,list);
    printf("value.i = %d, ", datap->value.i);
  } putchar('\n');

  up_list_foreach_reverse(item1, &data_list1) {
    datap = container_of(item1, up_value,list);
    printf("value.i = %d, ", datap->value.i);
  } putchar('\n');

  return EXIT_SUCCESS;
}


#include "up/value.h"

#include <stdlib.h>

static UP_DEFLIST(up_value_free_list);

//recyklujeme nebo alokujeme
up_value *up_value_new() {
    up_value *pvalue;
    if (up_list_empty(&up_value_free_list)) {
        pvalue = malloc(sizeof *pvalue);
        up_list_init(&pvalue->list);
    } else {
        pvalue = up_list_typepop(&up_value_free_list, up_value,list);
    }
    return pvalue;
}

void up_value_free(up_value *pvalue) {
    up_list_prepend(&up_value_free_list, &(pvalue->list));
}

up_value *up_value_new_double(double d) {
  up_value *pvalue;
  pvalue = up_value_new();
  pvalue->type = UP_VALUE_DOUBLE;
  pvalue->value.d = d;
  return pvalue;
}

up_value *up_value_new_pointer(void *p) {
  up_value *pvalue;
  pvalue = up_value_new();
  pvalue->type = UP_VALUE_POINTER;
  pvalue->value.p = p;
  return pvalue;
}

#define up_value_new_i(i) (up_value_new()->value.i=(i)) //jeste nastavit tag/type, list?

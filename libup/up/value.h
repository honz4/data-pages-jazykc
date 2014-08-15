#ifndef VALUE_H_INCLUDED
#define VALUE_H_INCLUDED
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "up/list.h"

//todo enum pro typy, makra new_i,l,ll... pro typy destruktor?/dispose callback?
struct up_value {
  int type;//type tag, a/nebo destruktor
  struct up_list    list;//< list, stack, up_value_free_list
  union {
  int        i;
  long       l;
  long long  ll;
  double     d;
  char      *z;
  void      *p;
  } value;
};

typedef struct up_value up_value;

#define UP_VALUE_DOUBLE  3;
#define UP_VALUE_POINTER 5;


up_value *up_value_new();
void      up_value_free(up_value *pvalue);
up_value *up_value_new_double(double d);
up_value *up_value_new_pointer(void *p);

#ifdef __cplusplus
}
#endif
#undef VALUE_H_INCLUDED

/** @file
 *
 * @todo MSVC ma __inline?
 */
#ifndef UP_LIST_H_INCLUDED
#define UP_LIST_H_INCLUDED
#endif
// vim: fenc=utf-8

#include "up/portable.h"

#ifdef __cplusplus
extern "C" {
#endif

/** list node i head budou mit stejnou strukturu.
 *
 * tato struktura je vnořena uvnitr další struct, která se tímto stane list-ovatelná
 *
 * manipulace s .next a .prev je "generická", použijeme static inline a/nebo makra
 */
struct up_list {
struct up_list *next; /**< dalsi node/head v seznamu. */
struct up_list *prev; /**< predchozi node/head v seznamu. */
};

/** definuje a inicializuje proměnnou, prázdný list head
 *
 * note: prázdný ukazuje sám na sebe (mohl by mít .next, .prev rovno NULL?)
 */
#define UP_DEFLIST(varname) struct up_list varname = { &varname, &varname }

/** inicializuj prázdný list head, static inline misto makra UP_DEFLIST()
 */
static inline void up_list_init (struct up_list *list) {
  list->next = list->prev = list;
}

/** je seznam list prázdný? proč neinline-uje??? protože v if? udělat makro!?
 */
static inline int up_list_empty (struct up_list *list) {
  return list->next == list;
}

/** pripoj item za list.
 *
 * @return item
 */
static inline struct up_list* up_list_append (struct up_list *list, struct up_list *item) {
  item->next = list->next;
  item->prev = list;
  list->next->prev = item;
  return list->next = item;
}

/** pripoj item před list, pokud je list hlava seznamu, jde o append na konec list.
 *
 * @note u libusual jsou 'logika'/nazvy _prepend a _append prohozené?
 * @return vrací item, nový ocas?
 */
static inline struct up_list* up_list_prepend (struct up_list *list, struct up_list *item) {
  item->next = list;
  item->prev = list->prev;
  list->prev->next = item;
  return list->prev = item;
}

/** odstran (vypoj) item z list.
 *
 * @return vrací item, vypojený prvek
 */
static inline struct up_list* up_list_remove (struct up_list *item) {
  item->prev->next = item->next;
  item->next->prev = item->prev;
  return item->next = item->prev = item;//item vy-empty-ujeme
}

/** pop: odstran (vypoj) item z list a vrat.
 *
 * @return vrací vypojený prvek, hlavu list nebo NULL
 */
static inline struct up_list* up_list_pop (struct up_list *list) {
  if (up_list_empty(list)) return NULL;
  return up_list_remove(list->next);
}

/** first prvek z list
 *
 * @return vrací první prvek seznamu
 */
static inline struct up_list* up_list_first (struct up_list *list) {
  if (up_list_empty(list)) return NULL;
  return list->next;
}

/** last prvek z list
 *
 * @return vrací poslední prvek seznamu
 */
static inline struct up_list* up_list_last (struct up_list *list) {
  if (up_list_empty(list)) return NULL;
  return list->prev;
}

/** type je struktura mající clena up_list field;
 */
#define up_list_typepop(list, type, field)\
  (up_list_empty(list) ? NULL\
   : container_of(up_list_remove((list)->next), type, field))

/** loop pres seznam, vlastně "hlavicka" for(;;), telo doda volajici,
 * item je pomocna promenna pro aktualni prvek, iterator? musi byt uz definována
 */
#define up_list_foreach(item, list)\
  for((item) = (list)->next; (item) != (list); (item) = (item)->next)

#define up_list_foreach_reverse(item, list)\
  for((item) = (list)->prev; (item) != (list); (item) = (item)->prev)

/** safe: item lze zrusit, potrebuje dalsi pomocnou tmp! */
#define up_list_foreach_safe(item, list, tmp)\
  for((item) = (list)->next, (tmp) = (list)->next->next;\
      (item) != (list);\
      (item) = (tmp), (tmp) = (tmp)->next)

#define up_list_foreach_reverse_safe(item, list, tmp)\
  for((item) = (list)->prev, (tmp) = (list)->prev->prev;\
      (item) != (list);\
      (item) = (tmp), (tmp) = (tmp)->prev)

/** typedef pro comparator funkci. */
typedef int (*up_list_compare_f)(const struct up_list *a, const struct up_list *b);

struct up_list *up_list_merge(up_list_compare_f cmp_func, struct up_list *p, struct up_list *q);
void up_list_sort (struct up_list *list, up_list_compare_f cf);

#ifdef __cplusplus
}
#endif
#undef UP_LIST_H_INCLUDED

#include "up/list.h"

/** @todo TODO implementovat sort, merge ... zkusit timsort? */

/*
static struct List *merge(list_cmp_f cmp_func, struct List *p, struct List *q)
{
        struct List res, *tail = &res, *e;

        while (p && q) {
                if (cmp_func(p, q) <= 0) {
                        e = p;
                        p = p->next;
                } else {
                        e = q;
                        q = q->next;
                }
                tail->next = e;
                tail = e;
        }
        tail->next = p ? p : q;
        return res->next;
}
*/

/** merge 2 ordered arrays into one */
struct up_list *up_list_merge(up_list_compare_f cmp_func, struct up_list *p, struct up_list *q) {
        struct up_list res, *tail = &res, *e;

        //dokud v seznamech neco zbyva:
        while (p && q) {
                //podle compate vezmeme, posuneme:
                if (cmp_func(p, q) <= 0) {
                        e = p;
                        p = p->next;
                } else {
                        e = q;
                        q = q->next;
                }
                //zapojime na ocas a ocas posuneme:
                tail->next = e;
                tail = e;
        }
        tail->next = p ? p : q;//zbytek
        return res.next;//vracime hlavu
}


/** non-recursive merge sort
 *
 * uses singly-linked NULL-terminated arrays internally.
 */
void up_list_sort(struct up_list *list, up_list_compare_f cmp_func) {
        int i, top = 0;
        struct up_list *p;
        struct up_list *stack[64];//postaci na 2^64 prvkovy vstup?

        if (up_list_empty(list))
                return;

        /* merge small sorted fragments into larger ones */
        while (list->next != list/*noempty*/) {
                p = list->next;
                list->next = p->next;
                p->next = NULL;

                for (i = 0; (i < top) && stack[i]; i++) {
                        p = up_list_merge(cmp_func, stack[i], p);
                        stack[i] = NULL;
                }

                stack[i] = p;
                if (i == top)
                        top++;
        }

        /* merge remaining fragments */
        for (p = NULL, i = 0; i < top; i++)
                p = up_list_merge(cmp_func, stack[i], p);

        /* restore proper List */
        list->next = p;
        for (p = list; p->next; p = p->next)
                p->next->prev = p;
        list->prev = p;
        p->next = list;
}

//todo: up_list_bsearch() na up_list_sort()-ovaný seznam? hmm, lepe seznam s poctem prvku!!!

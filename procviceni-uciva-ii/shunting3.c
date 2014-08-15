#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //isdigit() a spol
#include <math.h> //M_E, M_PI

//todo dynamika!
//todo stacky jako seznamy/list DONE
//todo mpint, mpdecimal
//todo debug vypisy

///  * number-token: push na number-stack, RPN: vystupni fronta
///  * operator: 
///    * right assoc: stop pokud dosáhneme operátoru se stejnou preferencí
///  * '(' levá závorka: push
///  * ')' pravá: pop operátory (+ vyhodnotit) až po odpovídající '(', závorky zahodit
///  * pokud vše načteno, vyhodnotit zbytek operátorů

#include "shunting.h"

//todo: tabulku definovat dynamicky, qsort + bsearch nebo statickou hash?
//todo: unarni operator
struct up_operator {
  int   op;
  int   prec;//<precedence/priority
  int   assoc;//<asociativita/unary flag/variable flag
  struct up_list list;//<seznam operatorů nebo free_list
  up_evalProc eval;
};

//typedef struct up_operator  *up_operator;

static UP_DEFLIST(up_operator_free_list);

//recyklujeme (free_list) nebo alokujeme
struct up_operator *up_operator_new(int op, int prec, int assoc, up_evalProc eval) {
  up_operator pop;
  if (up_list_empty(&up_operator_free_list)) {
     pop = malloc(sizeof *pop);
     pop->op    = op;
     pop->prec  = prec;
     pop->assoc = assoc;
     pop->eval  = eval;
     up_list_init(&pop->list);
  } else {
     pop = up_list_typepop(&up_operator_free_list, struct up_operator,list);
  }
  return pop;
}

void up_operator_free(up_operator pop) {
  up_list_prepend(&up_operator_free_list, &(pop->list));
}

// stack_push(stack, item);
// stack_pop(stack) => item
// stack_top(stack) => item
// stack_empty(stack)

typedef struct up_evaluator *up_evaluator;

struct up_evaluator {
  struct up_list numstack; //<zasobník operandů, nestaci pointer?
  struct up_list opstack;  //<zasobník operátorů
  up_operator symbols[128];//definovane operator a promenne, TODO take up_list: sort,bsearch
};

up_evaluator up_evaluator_new() {
  up_evaluator etor;
  etor = calloc(1, sizeof *etor);//symbols[] budou NULL
  up_list_init(&etor->numstack);
  up_list_init(&etor->opstack);
  return etor;
};

#define etor_get_op(etor, op) ((etor)->symbols[op])

up_operator etor_set_op(up_evaluator etor, up_operator op) {
  up_operator op1;
  op1 = etor->symbols[op->op];//predchozi operator, free? refcount? up_list_init()? TODO!
  etor->symbols[op->op] = op;
//printf("%s: ['%c'] %p\n",__func__,op->op,op);
  return op1;//todo, vracime predchozi XXX
}

void etor_opstack_print (up_evaluator etor) {
  struct up_list *item;
  up_operator op;
  putchar('o');
  up_list_foreach(item, &etor->opstack) {
    op = container_of(item, up_value,list)->value.p;
    printf("'%c' ",op->op);
  } putchar('\n');
}

//todo: funguje pouze na value.d double, dodelat up_value .to_string() !?
void etor_numstack_print (up_evaluator etor) {
  struct up_list *item;
  up_value *valuep;
  up_list_foreach(item, &etor->numstack) {
    valuep = container_of(item, up_value,list);
    printf("%g, ",valuep->value.d);
  } putchar('\n');
}

static up_operator etor_opstack_push(struct up_list *opstack, up_operator op) {
  up_value *valuep;
  valuep = up_value_new();
  up_list_append(opstack, &valuep->list);
  return valuep->value.p = op;
}

static void etor_numstack_push(up_evaluator etor, up_value *pvalue) {
  up_list_append(&etor->numstack, &pvalue->list);
}

up_value* etor_numstack_pop(up_evaluator etor) {
    return up_list_typepop(&etor->numstack, up_value,list);
}


void shunt_op (up_evaluator etor, up_operator op) {
  up_operator pop;//popped operator
  up_value *v1, *v2;
//printf("%s: '%c' %x\n", __func__, op->op, op->prec);
//etor_opstack_print(etor);
#if 1
//#define opstack_push(op) up_list_append(&((etor)->opstack), up_value_new()->value.p=(op))
#define opstack_push(op) etor_opstack_push(&((etor)->opstack), (op))
//pop je jako up_list_typeof(), ale nepotrebujeme kontrolu empty()
#define opstack_pop() ((up_operator)(container_of(up_list_remove((etor)->opstack.next), up_value,list)->value.p))//free!!!
#define opstack_top() ((up_operator)(container_of((etor)->opstack.next, up_value,list)->value.p))
#define opstack_empty() up_list_empty(&((etor)->opstack))

#define numstack_pop()   etor_numstack_pop((etor))
#define numstack_push(i) etor_numstack_push((etor),(i))
#endif
  if (op->assoc & KONST) {//uloz konstantu/promennou na numstack
     numstack_push(op->eval(NULL,NULL));
     return;
  } else if (op->assoc & ASSOC_VAR) {//uloz promennou na numstack, kopie/reference?
     numstack_push(up_value_new_pointer(op->eval));//XXX funguje akorat na matice
     return;
  }

  switch (op->op) {
  case '(': //push na opstack
    opstack_push(op);
    return;
  case ')': //vyhodnotit az po '('
    while (!opstack_empty() && opstack_top()->op != '(') {
      pop = opstack_pop();
      v2 = numstack_pop();
      if (pop->prec & UNARY) {
        v1 = v2, v2 = NULL;
      } else {
        v1 = numstack_pop();
      }
      numstack_push(pop->eval(v1, v2));
    }
    //ted musi na opstack lezet '('
    if (!(pop=opstack_pop()) || pop->op != '(') {
       //todo: chybova hlaseni s line:col
       exit(2);
    }
    return;
  default://zpracuj operator
    if (op->assoc & ASSOC_RIGHT) {
       while (!opstack_empty() && op->prec < opstack_top()->prec) {
         exit(3);
       }
    }
    else {//ASSOC_LEFT: pokud maji operatory na opstacku vetsi/rovno prioritu:
       while (!opstack_empty() && op->prec <= opstack_top()->prec) {
         pop = opstack_pop();
         v2 = numstack_pop();
         if (pop->assoc & UNARY) {
           v1 = v2, v2 = NULL;
         } else {
           v1 = numstack_pop();
         }
         numstack_push(pop->eval(v1, v2));
//printf("%c->eval(%g, %g)\n", pop->op, v1->value.d, v2->value.d);//todo vypis numstack_top()
       }
    }
  }
  opstack_push(op);
}

up_value* etor_eval (up_evaluator etor, char *expr) {
  int line=1, col=1;
  char *tstart = NULL;
  char *endptr;
  double num;
  up_value *pvalue;
  up_operator op;

//todo: ma etor prazdne stacky?
  shunt_op(etor, etor_get_op(etor,'('));//vyraz ozavorkujeme
//primitivni lexer:
  for (; *expr; expr++) {
    if (isspace(*expr)) {
     //bile znaky nic, jen hlidame konec radku
       if (*expr=='\n') { line++; col=1; }
       //printf("%d:%d:ws\n",line,col);
    } else if (isdigit(*expr)) {
     //cislice: nastavime token start
       if (!tstart) tstart=expr;
       //printf("%d:%d:'%c'\n",line,col,*expr);
    } else if ( (op=etor_get_op(etor,(int)*expr)) ) {
     //narazili jsme na operator
     //printf("%d:%d:op=%c\n",line,col,*expr);
     //nacteme predchozi cislici, pouzijeme strtod()
       if (tstart) {
       num = strtod(tstart, &endptr); tstart=NULL;
       //printf("push num=%g\n", num);
       numstack_push(up_value_new_double(num));
       }
       shunt_op(etor, op);
    } else {//
       fprintf(stderr, "%s:%d:%d:'%c' !!!ani bile znaky, ani digit, ani operator???\n",__func__,line,col,*expr);
       exit(EXIT_FAILURE);
    }
    col++;
  }
  //flush:
       if (tstart) {
       num = strtod(tstart, &endptr); tstart=NULL;
       //printf("push num=%g\n", num);
       numstack_push(up_value_new_double(num));
       }
// etor_numstack_print(etor);
//  etor_opstack_print(etor);
//printf("------------\n");
  shunt_op(etor, etor_get_op(etor,')'));//vyraz ozavorkujeme
  pvalue = numstack_pop();//mame vse empty?
  return pvalue;
}

//todo: shunt_postfix (RPN) a shunt_prefix (Polish Notation)

#ifndef NO_MAIN

static char *expr = " 30 + 40 + 5 - (3 + 4)*55"; // bc spocte -310
//static char *expr = " 30 + 40";
//static char *expr = " 30 - 40 - 10";
//static char *expr = " 30 + 40 + 10";
//static char *expr = "2^20";
//static char *expr = "e *10";

static up_value*  eval_e(up_value *op1, up_value *op2) {
  return up_value_new_double(M_E);
}

//todo: vracet up_value/kod?, TODO: typovou koerci pomoci up_value.type...
//todo: nebo 1. parametr etor a vracet rovnou na .numstack?
static up_value* eval_mul (up_value *op1, up_value *op2) {
  //na kontrolu .type a koerci kasleme?
  return up_value_new_double(op1->value.d * op2->value.d);
}

static up_value* eval_add (up_value *op1, up_value *op2) {
  return up_value_new_double(op1->value.d + op2->value.d);
}

static up_value* eval_sub (up_value *op1, up_value *op2) {
  return up_value_new_double(op1->value.d - op2->value.d);
}

static up_value* eval_pow (up_value *op1, up_value *op2) {
  return up_value_new_double( pow(op1->value.d, op2->value.d) );
}

int main (int argc, char *argv[]) {
  up_evaluator etor;

  etor = up_evaluator_new();//uz inicializovat ( a )?
  etor_set_op(etor, up_operator_new('(', 0, ASSOC_NONE,   NULL));
  etor_set_op(etor, up_operator_new(')', 0, ASSOC_NONE,   NULL));
  etor_set_op(etor, up_operator_new('*', 8, ASSOC_LEFT ,eval_mul));
  etor_set_op(etor, up_operator_new('+', 5, ASSOC_LEFT,   eval_add));
  etor_set_op(etor, up_operator_new('-', 5, ASSOC_LEFT,   eval_sub));
  etor_set_op(etor, up_operator_new('^',10, ASSOC_RIGHT,  eval_pow));
  etor_set_op(etor, up_operator_new('e', 0, KONST     ,   eval_e));

  printf("eval \"%s\" => %g\n", expr, etor_eval(etor, expr)->value.d);

  return EXIT_SUCCESS;
}
#endif

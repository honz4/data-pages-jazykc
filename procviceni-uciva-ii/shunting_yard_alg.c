#include <stdio.h>
#include <stdlib.h>

//todo dynamika!
//todo stacky jako seznamy/list
//todo mpint, mpdecimal
//todo debug vypisy

///  * number-token: push na number-stack, RPN: vystupni fronta
///  * operator: 
///    * right assoc: stop pokud dosáhneme operátoru se stejnou preferencí
///  * '(' levá závorka: push
///  * ')' pravá: pop operátory (+ vyhodnotit) až po odpovídající '(', závorky zahodit
///  * pokud vše načteno, vyhodnotit zbytek operátorů

enum { ASSOC_NONE=0,ASSOC_LEFT=1,ASSOC_RIGHT=2,  UNARY=0x10};

static int eval_mul (int op1, int op2) { return op1 * op2; }
static int eval_add (int op1, int op2) { return op1 + op2; }
static int eval_sub (int op1, int op2) { return op1 - op2; }

/* celociselna mocnina */
#include "up/math.h" /* ipow() */
static int eval_pow (int op1, int op2) { return ipow(op1, op2); }

//todo: tabulku definovat dynamicky, qsort + bsearch nebo statickou hash?
//todo: lze '\0' EOF pojmout jako operator s nejvyssi prioritou?
//todo: unarni operator
struct op_s {
  char op;
  int  prec;
  int  assoc;// ASSOC_*|UNARY
  int (*eval)(int op1, int op2);
} operatory[]={
 {'*', 8, ASSOC_LEFT,  eval_mul}
,{'+', 5, ASSOC_LEFT,  eval_add}
,{'-', 5, ASSOC_LEFT,  eval_sub}
,{'^',10, ASSOC_RIGHT, eval_pow}
,{'(', 0, ASSOC_NONE, NULL}
,{')', 0, ASSOC_NONE, NULL}
};

//vyhleda operator v operatory[]
struct op_s * getop (char op) {
  int i;
  for (i=0; i < sizeof operatory / sizeof *operatory; i++) {
      if (op == operatory[i].op) {
         return operatory + i;
      }
  }
  return NULL;//nenasli!
}

//TODO zp_stack.{c,h}
//zatim udelame dva zasobniky 'natvrdo' + makra
#define STACK_SIZE 100
int numstack[2*STACK_SIZE];//umistit SP na stack[0]???
int numstackp=-1;//stackpointer
//TODO kontrola preteceni?
#define numstack_push(num) (numstack[++numstackp]=(num))
#define numstack_pop() (numstack[numstackp--])

static struct op_s* opstack[STACK_SIZE] = {0}; //stackpointer je [0]
static int opstackp=-1;
//TODO kontrola preteceni?
#define opstack_push(op)  (opstack[++opstackp]=(op))
#define opstack_pop()     (opstackp>=0 ? opstack[opstackp--] : NULL)
#define opstack_top()     (opstack[opstackp])
#define opstack_size()    (opstackp+1)
#define opstack_empty()   (opstackp<0)
void opstack_print() { int i;
printf("ops#%d: \n",opstackp);
for (i=0; i<opstackp; i++) { printf("'%c'[%d]\n",opstack[i]->op,i);}
}

void shunt_op (up_operator op) {
  up_operator pop;//popped operator
  int n1, n2;
printf("%s: '%c' %x\n", __func__, op->op, op->prec);
  switch (op->op) {
  case '(': //push na opstack
    opstack_push(op);
    return;
  case ')': //vyhodnotit az po '('
    while (!opstack_empty() && opstack_top()->op != '(') {
      pop = opstack_pop();
      n2 = numstack_pop();
      if (pop->prec & UNARY) {
        n1 = n2, n2 = 0;
      } else {
        n1 = numstack_pop();
      }
      numstack_push(pop->eval(n1, n2));
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
         n2 = numstack_pop();
         if (pop->assoc & UNARY) {
           n1 = n2, n2 = 0;
         } else {
           n1 = numstack_pop();
         }
printf("%c->eval(%d, %d) => %d\n", pop->op, n1, n2,
         numstack_push(pop->eval(n1, n2)));
       }
    }
  }
  opstack_push(op);      
}
//todo: shunt_postfix (RPN) a shunt_prefix (Polish Notation)

//char *expr = " 30 + 40 + 5 - (3 + 4)*55"; // bc spocte -310
//char *expr = " 30 + 40";
//char *expr = " 30 + 40 + 10";
char *expr = "2^20";

int main (int argc, char *argv[]) {
  char *tstart = NULL;
  struct op_s *op;
  int  num, n1, n2;
  int line=1, col=1;

  printf("expr=%s\n", expr);
//primitivni lexer
  for (; *expr; expr++) {
    if (isspace(*expr)) {
     //bile znaky nic, jen hlidame konec radku
       if (*expr=='\n') { line++; col=1; }
       printf("%d:%d:ws\n",line,col);
    } else if (isdigit(*expr)) {
     //cislice: nastavime token start
       if (!tstart) tstart=expr;
       printf("%d:%d:'%c'\n",line,col,*expr);
    } else if (op=getop(*expr)) {
       //narazili jsme na operator
       printf("%d:%d:op=%c\n",line,col,*expr);
       //nacteme predchozi cislici
       if (tstart) {
       num = atoi(tstart); tstart=NULL;
       printf("push num=%d\n", num);
       numstack_push(num);
       }
       shunt_op(op);
    } else {//ani bile znaky, ani operator???
       printf("%s:%d:%d:'%c'???\n",argv[0],line,col,*expr);
    }
    col++;
  }
  //flush: '\0' bude EOF "operator"?
       if (tstart) {
       num = atoi(tstart); tstart=NULL;
       printf("push num=%d\n", num);
       numstack_push(num);
       }
  opstack_print();
  //vyprazdnime op+num stack, pokud mame EOF 'operator' netreba?
  while (!opstack_empty()) {
    op = opstack_pop();
    n2 =numstack_pop();
    if (op->assoc & UNARY) { n1=n2,n2=0; } else { n1 = numstack_pop(); }
printf("%c.eval(%d, %d) => %d\n", op->op, n1, n2,
    numstack_push(op->eval(n1, n2)));
  }
  printf("result=%d\n", numstack[0]);
  return EXIT_SUCCESS;
}

/** @file

Procvičení_učiva_II
Katedra informatiky Univerzity Palackého v_Olomouci

****** Maticová kalkulačka ******
Téma: Procvičení učiva II
Procvičované učivo: práce s textovým souborem, celková koncepce programu,
dynamická vícerozměrná pole, strukturované datové typy

Napište v jazyku C program pro výpočty libovolných formulí složených z matic
(velká písmena anglické abecedy),
znamének plus a krát (pro operace sčítání a násobení matic) a závorek.
Program bude zadání výpočtu (formuli i matice) načítat ze vstupního textového souboru
a výslednou matici ukládat do výstupního textového souboru.

Jména vstupního a výstupního souboru (případně cestu k souborům)
bude možné specifikovat přímo při spuštění programu z příkazové řádky.

V programu by také měla být zabudována kontrola správnosti načteného zadání a
ošetření možných chyb (při otevírání souboru, alokaci paměti a podobně).

Program by měl být napsaný přehledně a efektivně - s důrazem na smysluplné
rozdělení do několika funkcí a minimum globálně deklarovaných proměnných.

Příklad použití:
./kalkulacka zadani.txt vysledek.txt (OS Linux)
kalkulacka.exe zadani.txt vysledek.txt (OS Windows)
Příklad vstupního souboru:
(A * B)+(C*(A*B))

2 3
1.1 1 2
4 5 6

3 4
1 2 3 4
5 6 7 8
9 0 1 2

2 2
1 2
3 4
Příklad výstupního souboru:
2 4
214.2  92.4 130.6 168.8
487.3 214.6 301.9 389.2
Povolené knihovny: stdio.h, stdlib.h, string.h
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shunting.h"

//FIXME #include "matice.h"
struct matice;//neuplny typ
typedef struct matice *matice;

struct matice *matice_new(unsigned m, unsigned n);
double *matice_data(matice A);
size_t  matice_mxn(matice A,unsigned *m, unsigned *n);
void    matice_print(matice A);//vracet pocet vypsanych znaku?
int     matice_add(matice A, matice B, matice *C);
int     matice_mul(matice A, matice B, matice *C);


//todo: umet nasobit matici cislem?
//todo: kontrola .type na matice
static up_value* eval_mul (up_value *op1, up_value *op2) {
  int items;
  up_value *pvalue;
  matice C = NULL;
  items = matice_mul(op1->value.p, op2->value.p, &C);
  if (!items)
     return NULL;
  pvalue = up_value_new();
  pvalue->value.p = C;
  return pvalue;
}

static up_value* eval_add (up_value *op1, up_value *op2) {
  up_value *pvalue;
  matice C = NULL;

  matice_add(op1->value.p, op2->value.p, &C);
//matice_print(C);
  pvalue = up_value_new();
  pvalue->value.p = C;
  return pvalue;
}

//static char *expr = "(A * B)+(C*(A*B))";
static char *expr = "A+A";

int main(int argc, char *argv[]) {
  ssize_t readed;
  char *linebuf = NULL;
  size_t  linelen;//kapacita linebuf
  unsigned m, n;//rozmery matice
  int Mname;//jmeno matice 'A'..'Z'
  up_value *pvalue;
  up_evaluator etor;

  if (argc > 1) {
     stdin = freopen(argv[1], "r", stdin);
//prvni radek je maticovy vyraz
     readed = getline(&linebuf, &linelen, stdin);//vraci -1 pokud chyba/EOF
     expr = strdup(linebuf);
  }

  etor = up_evaluator_new();
  etor_set_op(etor, up_operator_new('(', 0, ASSOC_NONE,   NULL));
  etor_set_op(etor, up_operator_new(')', 0, ASSOC_NONE,   NULL));
  etor_set_op(etor, up_operator_new('*', 8, ASSOC_LEFT ,eval_mul));
  etor_set_op(etor, up_operator_new('+', 5, ASSOC_LEFT,   eval_add));

  Mname='A';
  do {
     readed = getline(&linebuf, &linelen, stdin);//vraci -1 pokud chyba/EOF
   //printf("%d#%s", strlen(linebuf), linebuf);
     if (2 == sscanf(linebuf, "%u %u", &m, &n)) {//matice start
        int i,j,skip;
        matice M;
        double *pdata;
        char *pch;

        //printf("m=%u n=%u\n",m,n);
        M = matice_new(m,n);
        pdata = matice_data(M);
        if (1) {
        for (i=0; i<m; i++) { //radky matice
            readed = getline(&linebuf, &linelen, stdin);
            for(j=0, pch=linebuf; j<n; j++, pdata++, pch += skip) {
              sscanf(pch," %lg%n",pdata,&skip);//kontrola 1==
            //printf("%g[%d,%d]|%d\"%s",*pdata,i,j,skip,pch);
            }
        }}
        etor_set_op(etor, up_operator_new(Mname, 0, ASSOC_VAR, (up_evalProc)M));
/*      size_t items = matice_mxn(M, &m, &n);
        //printf("matice %c(%u,%u) items=%zu\n", Mname, m,n, items); // */
        //matice_print(M);
        Mname++;
     }
  } while (readed != -1);

  fprintf(stderr, "eval %s =>\n", expr );
  pvalue = etor_eval(etor, expr);
  matice_print(pvalue->value.p);

  return EXIT_SUCCESS;
}

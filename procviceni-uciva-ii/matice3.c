#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//don't edit! generated from ./matice-template.c

///zajímavé odkazy okolo matic
///  * http://www.abclinuxu.cz/zpravicky/acm-sigplan-ocenila-gcc#12


#include "up/list.h"

//TODO: header file + opaque
struct matice {
  unsigned m;//<pocet radku
  unsigned n;//<pocet slopcu nebo list/darray pro rozmery?
  struct up_list list;//<vyuzijeme treba na zasobnik, freeList...
  size_t items; // (m * n) <= items, resp. pocet alokovanych, invariant: m*n <= items
  //udelat to jako sds? header+data
  double data[];//C99 flexible array
};

typedef struct matice *matice;

struct matice *matice_new(unsigned m, unsigned n) {
  struct matice *pmatice;
//todo: nelze recyklovat nejakou ve free listu?
  pmatice = calloc(sizeof *pmatice + m * n * sizeof(double), 1);
  pmatice->m = m;
  pmatice->n = n;
  up_list_init(&pmatice->list);
  pmatice->items = m*n;
  return pmatice;
}

double *matice_data(matice A){return A->data;}

size_t matice_mxn(matice A,unsigned *m, unsigned *n){*m=A->m; *n=A->n; return A->items;}

void matice_free(struct matice *pmatice) {
//todo: recyklovat ve freelistu!
  free(pmatice);
}

/** secte matice A, B vysledek ulozi do pripadne nove alokovane C
 */
int matice_add(matice A, matice B, matice *C) {
  unsigned i, items;
  double *Arow, *Brow, *Rrow;
//"kontrola" pomoci assert(), todo error code kontrola?
  items = A->m * A->n;
  assert(A->m == B->m && A->n == B->n);
  if (*C) {//volajici dodal matici pro vysledek, kapacita je dostatecne?
      assert((*C)->items >= items);
  }
  else {
     *C = matice_new(A->m, A->n);
  }
  Arow = A->data;
  Brow = B->data;
  Rrow = (*C)->data;
//vubec nemusime for(i=...) for(j=...), secteme to jako jednorozmerne pole
//pomoci ukazatele na radek
  for (i=0; i < items; i++) {
     *Rrow++ = *Arow++ + *Brow++;
  }
  return 0;// 0==OK error code
}

int matice_mul(matice A, matice B, matice *C) {
  unsigned i, j, k, items;
  double *Arow, *Bcol, *row, *col, *out, sum;
  assert(A->n == B->m);
  items = A->m * B->n;
  if (*C) {//volajici dodal matici pro vysledek, kapacita je dostatecne?
      assert((*C)->items >= items);
      (*C)->m = A->m;
      (*C)->n = B->n;
  }
  else {
     *C = matice_new(A->m, B->n);
  }
//prachsproste nasobeni O(n^3) matic A(m,n) X B(n,p) = C(m,p)
  out = (*C)->data;
  row = Arow = A->data;
  for (       i=0; i < A->m; i++) {
  col = Bcol = B->data;
  for (       k=0; k < B->n; k++,  row = Arow, col = ++Bcol) {
  for (sum=0, j=0; j < A->n; j++, row++, col += B->n) {
       sum += *row * *col;
     //printf("[%u,%u] %u   + %g *%g -> %g \n",i,k,j, *row, *col, sum);
  }
  *out++ = sum;
  }
  row = (Arow += A->n);
  }
  return items;
}

//transponuje matici A do C */
int matice_t (matice A, matice *C) {
  unsigned items, i, j;
  double *Arow, *Bcol;
  items = A->m * A->n;
  if (*C) {//volajici dodal matici pro vysledek, kapacita je dostatecne?
     assert((*C)->items >= items);
  }
  else {
    *C = matice_new(A->n, A->m);
  }
  Arow = A->data;
  for(i=0; i < A->m; i++) {
  Bcol = (*C)->data + i;
  for(j=0; j < A->n; j++) {
    *Bcol = *Arow++;
     Bcol += A->m;
  }}
  return items;
}



//matice_info(matice A, int *m, int *n)
void matice_print(matice A) {
  unsigned i,j;
  double *data;
  printf("%u %u\n", A->m, A->n);
  data = A->data;
  for(i=0; i < A->m; i++) {
  for(j=0; j < A->n; j++) {
    printf("%g ", *data++);
  } putchar('\n');
  }
}
//matice_save()
//matice_load()
//nejaky format pro ukladani/nacitani matic? Matrix Market?

//test main, todo testy! libtap?
#ifndef NO_MAIN
int main(int argc, char *argv[]) {
  int i,j;
  int m,n;
  matice A, B, C, A_T, D, E;
  matice C0 = NULL;
  m=3; n=5;
  A = matice_new(m, n);//m radky, n sloupcu
  B = matice_new(m, n);
  C = matice_new(m, n);
  A_T = matice_new(n, m);
  for(i=0; i<m; i++) {
  for(j=0; j<n; j++) {
   A->data[i*n + j] = i*n + j;
   B->data[i*n + j] = i + 100*j;
  }}
  matice_print(A);
  matice_print(B);
  matice_add(A,B, &C); matice_print(C);
  matice_add(A,A, &C0); matice_print(C0);
  matice_add(B,B, &C); matice_print(C);
//s transponovanou to urcite pujde nasobit:
  matice_t(A,&A_T); matice_print(A_T);

  D = matice_new(2, 3);
  E = matice_new(3, 2);
  for(i=0; i<2*3; i++) { D->data[i] = i+1; } //(2,3)*(3,2) {{22,28},{49,64}}
  for(i=0; i<3*2; i++) { E->data[i] = i+1; } //(2,3)*(3,5) {{46,52,58,64,70},{100,115,130,145,160}}
  matice_print(D);
  matice_print(E);
  matice_mul(D, E, &C); matice_print(C);
  return 0;
}
#endif

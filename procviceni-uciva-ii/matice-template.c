#!/bin/sh
#jednoduchy generator pomoci http://en.wikipedia.org/Here_document
test "$1" || set -- double
case "${1}" in
long)     TYPE=$1 matice=maticel  FMT=ld  ;;
longlong) TYPE=$1 matice=maticell FMT=lld ;;
double)   TYPE=$1 matice=matice   FMT=g   ;;
*) echo "$0: TYPE=$1: podporujeme pouze long, longlong a double!!!" >&2 ; exit 1;; esac
cat << //MATICE_TEMPLATE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//don't edit! generated from $0

///zajímavé odkazy okolo matic
///  * http://www.abclinuxu.cz/zpravicky/acm-sigplan-ocenila-gcc#12


#include "up/list.h"

//TODO: header file + opaque
struct ${matice} {
  unsigned m;//<pocet radku
  unsigned n;//<pocet slopcu nebo list/darray pro rozmery?
  struct up_list list;//<vyuzijeme treba na zasobnik, freeList...
  size_t items; // (m * n) <= items, resp. pocet alokovanych, invariant: m*n <= items
  //udelat to jako sds? header+data
  ${TYPE} data[];//C99 flexible array
};

typedef struct ${matice} *${matice};

struct matice *${matice}_new(unsigned m, unsigned n) {
  struct ${matice} *pmatice;
//todo: nelze recyklovat nejakou ve free listu?
  pmatice = calloc(sizeof *pmatice + m * n * sizeof(${TYPE}), 1);
  pmatice->m = m;
  pmatice->n = n;
  up_list_init(&pmatice->list);
  pmatice->items = m*n;
}

void ${matice}_free(struct ${matice} *pmatice) {
//todo: recyklovat ve freelistu!
  free(pmatice);
}

/** secte matice A, B vysledek ulozi do pripadne nove alokovane C
 */
int ${matice}_add(${matice} A, ${matice} B, ${matice} *C) {
  unsigned i, items;
  ${TYPE} *Arow, *Brow, *Rrow;
//"kontrola" pomoci assert(), todo error code kontrola?
  items = A->m * A->n;
  assert(A->m == B->m && A->n == B->n);
  if (C) {//volajici dodal matici pro vysledek, kapacita je dostatecne?
      assert((*C)->items >= items);
  }
  else {
     *C = ${matice}_new(A->m, A->n);
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

int ${matice}_mul(${matice} A, ${matice} B, ${matice} *C) {
  unsigned i, j, k, items;
  ${TYPE} *Arow, *Bcol, *row, *col, *out, sum;
  assert(A->n == B->m);
  items = A->m * B->n;
  if (C) {//volajici dodal matici pro vysledek, kapacita je dostatecne?
      assert((*C)->items >= items);
      (*C)->m = A->m;
      (*C)->n = B->n;
  }
  else {
     *C = ${matice}_new(A->m, B->n);
  }
//prachsproste nasobeni O(n^3) matic A(m,n) X B(n,p) = C(m,p)
  out = (*C)->data;
  row = Arow = A->data;
  for (       i=0; i < A->m; i++) {
  col = Bcol = B->data;
  for (       k=0; k < B->n; k++,  row = Arow, col = ++Bcol) {
  for (sum=0, j=0; j < A->n; j++, row++, col += B->n) {
       sum += *row * *col;
     //printf("[%u,%u] %u   + %${FMT} *%${FMT} -> %${FMT} \n",i,k,j, *row, *col, sum);
  }
  *out++ = sum;
  }
  row = (Arow += A->n);
  }
  return items;
}

//transponuje matici A do C */
int ${matice}_t (${matice} A, ${matice} *C) {
  unsigned items, i, j;
  ${TYPE} *Arow, *Bcol;
  items = A->m * A->n;
  if (C) {//volajici dodal matici pro vysledek, kapacita je dostatecne?
     assert((*C)->items >= items);
  }
  else {
    *C = ${matice}_new(A->n, A->m);
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



//${matice}_info(${matice} A, int *m, int *n)
${matice}_print(${matice} A) {
  unsigned i,j;
  ${TYPE} *data;
  printf("%u %u\n", A->m, A->n);
  data = A->data;
  for(i=0; i < A->m; i++) {
  for(j=0; j < A->n; j++) {
    printf("%${FMT} ", *data++);
  } putchar('\n');
  }
}
//${matice}_save()
//${matice}_load()
//nejaky format pro ukladani/nacitani matic? Matrix Market?

//test main, todo testy! libtap?
#ifndef BUILD_libup
int main(int argc, char *argv[]) {
  int i,j;
  int m,n;
  ${matice} A, B, C, A_T, D, E;
  m=3; n=5;
  A = ${matice}_new(m, n);//m radky, n sloupcu
  B = ${matice}_new(m, n);
  C = ${matice}_new(m, n);
  A_T = ${matice}_new(n, m);
  for(i=0; i<m; i++) {
  for(j=0; j<n; j++) {
   A->data[i*n + j] = i*n + j;
   B->data[i*n + j] = i + 100*j;
  }}
  ${matice}_print(A);
  ${matice}_print(B);
  ${matice}_add(A,B, &C); ${matice}_print(C);
//s transponovanou to urcite pujde nasobit:
  ${matice}_t(A,&A_T); ${matice}_print(A_T);

  D = ${matice}_new(2, 3);
  E = ${matice}_new(3, 2);
  for(i=0; i<2*3; i++) { D->data[i] = i+1; } //(2,3)*(3,2) {{22,28},{49,64}}
  for(i=0; i<3*2; i++) { E->data[i] = i+1; } //(2,3)*(3,5) {{46,52,58,64,70},{100,115,130,145,160}}
  ${matice}_print(D);
  ${matice}_print(E);
  ${matice}_mul(D, E, &C); ${matice}_print(C);
  return 0;
}
#endif
//MATICE_TEMPLATE

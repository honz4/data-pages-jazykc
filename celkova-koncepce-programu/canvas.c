/** @file
 *
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "canvas.h"

/*
 [0][0], ...[0,width-1], [0,width]='\0'
 [1][0], ...[1,width-1], [1,width]='\0'

Poznamka: v canvas.h by melo stacit typedef struct canvas;
*/
typedef
struct canvas_s {
  int width;
  int height;
  char *canvas;/**< char canvas[height][width+1], 'linearizovane' 2rozm pole */
} canvas;


/** vytvori platno
 *
 * @param width sirka platna
 * @param height vyska platna
 * @return ukazatel na strukturu platna
 */
canvas *canvas_create(int width, int height) {
  canvas *pcanvas;
  pcanvas = malloc(sizeof *pcanvas);//note: sizeof *pcanvas misto sizeof(canvas)
  pcanvas->width = width;
  pcanvas->height = height;
  pcanvas->canvas = (char*)malloc(width * height + height/*kvuli koncovym '\0'*/);
  canvas_clear(pcanvas);
  return pcanvas;
}

/* nastavi dany bod na zadanou hodnotu */
void canvas_set_point(canvas *c, int x, int y, char character);

/* vrati znak daneho bodu */
int canvas_get_point(canvas *c, int x, int y);

/* nakresli obdelnik */
void canvas_draw_rect(canvas *c, int x, int y, int width, int height, char ch);

/* vycisti platno */
void canvas_clear(canvas *c) {
  int i;
  char *end;
//vyplnime mezerami:
  memset(c->canvas, '.', c->height * (c->width+1));//na sirce mame o znak vice
  for(i=0, end=c->canvas; i < c->height; i++) {
    end[c->width]='\0'; end += c->width + 1;
  }
}

/** vykresli obsah platna na standardni vystup
 *
 * @todo lze jako makro pomoci canvas_output()
 */
void canvas_print(canvas *c) {
  int i;
  char *line;
  for(i=0, line=c->canvas; i< c->height; i++) {
     printf("%s\n", line);
     line += c->width + 1;
  }
}

/* vykresli obsah platna do souboru */
void canvas_output(canvas *c, FILE *f) {
}


/** @file
 *
 */
#ifndef CANVAC_H_INCLUDED
#define CANVAC_H_INCLUDED
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**toto lze, 'prazdna' struktura, definovana az v canvas.c,
 * neni treba/vhodne zverejnovat!!!
 */
typedef struct canvas_s canvas;

/** vytvori platno */
canvas *canvas_create(int x, int y);

/** nastavi dany bod na zadanou hodnotu */
void canvas_set_point(canvas *c, int x, int y, char character);

/** vrati znak daneho bodu */
int canvas_get_point(canvas *c, int x, int y);

/** nakresli obdelnik */
void canvas_draw_rect(canvas *c, int x, int y, int width, int height, char ch);

/** vycisti platno */
void canvas_clear(canvas *c);

/** vykresli obsah platna na standardni vystup */
void canvas_print(canvas *c);

/** vykresli obsah platna do souboru */
void canvas_output(canvas *c, FILE *f);

#ifdef __cplusplus
}
#endif
#undef CANVAC_H_INCLUDED

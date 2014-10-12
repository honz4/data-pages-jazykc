/** @file

Celková_koncepce_programu

====== ASCII Art ======
Téma: Celková koncepce programu
Procvičované učivo: celková koncepce programu, dynamická práce s pamětí, funkce
Napište v jazyku C jednoduchou knihovnu funkcí pro vykreslování obrázků pomocí
znaků (tzv. ASCII art).

Knihovna by měla mít tyto vlastnosti:
    * Obrázky se budou vykreslovat pomocí plátna - dvojrozměrné matice, která
      bude obsahovat jednotlivé znaky.
    * Vykreslování se tedy neprovádí přímo na výstupu, ale pouze dochází ke
      změně daného plátna (struktura canvas).
    * Je možné pracovat současně s několika plátny.
    * Je možné "vykreslovat" i za hranicí kreslící plochy, tyto body se ale
      nebudou při zobrazení plátna vykreslovat. Jinými slovy, při pokusu o
      kreslení mimo plátno nedojde k vyjímce při běhu programu.
    * Knihovna by měla být samostatným modulem, bude tedy tvořena jedním
      zdrojovým a jedním hlavičkovým souborem.
V knihovně vytvořte strukturovaný datový typ canvas a dále definujte tyto funkce:
*/

/*
Jednotlivé funkce ve vytvořené knihovně poté otestujte z modulu hlavní funkce.
Příklad výstupu:
<code>
    xxxxxxx
    x     x
    x   ===========
    x   * x       *
    x   * x       *
    x   * x       *
    x   ===========
    x     x
    x     x
    xxxxxxx
</code>
Příklad výstupu:
<code>
     ===========
     *         *
     *  o   o  *
     *    |    *
     *         *
     *  \___/  *
     *         *
     ===========
</code>
*/
//Povolené knihovny:
#include <stdio.h>
#include <stdlib.h>

#include "canvas.h"


int main(int argc, char *argv[]) {
  canvas *platno;

  platno = canvas_create(30, 10);

  canvas_print(platno);


  exit(EXIT_SUCCESS);
}

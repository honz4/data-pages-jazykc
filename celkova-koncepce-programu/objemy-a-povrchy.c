/** @file
Objemy a povrchy

Téma: Celková koncepce programu
Procvičované učivo: celková koncepce programu, parametry funkce main, funkce, větvení

Napište v jazyku C program pro výpočet
  * objemu a povrchu válce,
  * pravidelného trojbokého,
  * čtyřbokého a
  * šestibokého hranolu.
Parametry výpočtu by mělo být možné předávat programu při spuštění z příkazové řádky.
Zdrojový kód programu by měl být rozdělen do 2 modulů.

Modul hlavní funkce (soubor main.c) bude
  * zajišťovat zpracování
  * a případně načtení chybějící parametrů výpočtu,
  * budou z něj volány funkce zajišťující vlastní výpočet
  * a vypisovány vypočítané hodnoty na obrazovku.
Note: misto main.c slouzi tento soubor!

Druhý modul (soubory vypocet.h a vypocet.c) pak bude zajišťovat
veškeré požadované výpočty. Při řešení úlohy dbejte všech zásad zmíněných na
přednášce.

Příklad použití:
objemy_a_povrchy.exe 0 1.2 2.4 (OS Windows)
./objemy_a_povrchy   0 1.2 2.4 (OS Linux)

Příklad výstupu:
Valec s vyskou 1.2 a polomerem podstavy 2.4 ma povrch 54.2592 a objem 21.7037.

Příklad použití:
objemy_a_povrchy.exe 3 2.3 4.5 (OS Windows)
./objemy_a_povrchy   3 2.3 4.5 (OS Linux)

Pravidelny 3-boky hranol s vyskou 2.3 a delkou podstavne hrany 4.5
ma povrch 48.587 a objem 20.1676.
*/
//Povolené knihovny:
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "vypocet.h"

int main(int argc, char *argv[]) {
  long n;// op kod, resp. pocet stran n-uhelnika
  char *endptr;//pro strtol()
  double r;//polomer nebo velikost strany
  double h;//vyska
  double objem;
  double povrch;
  if (argc <= 3) {
Usage:
     printf("Usage: %s 0|3|4|6  vyska  polomer|strana\n", argv[0]);
     exit(EXIT_FAILURE);
  }
//atoi(argv[1]);//pouzijeme strtol(), atoi() nedetekuje chybu!
//note: lze i sscanf(), format cteni ze stringu
  n = strtol(argv[1], &endptr, 10);
  if (*endptr) {
     printf("%s: spatny opkod/pocet\n", argv[1]);
     goto Usage;
  }

  h = strtod(argv[2], &endptr);
  if (*endptr) {//precteno beze zbytku?
     printf("%s: spatna vyska\n", argv[2]);
     goto Usage;
  }

  r = strtod(argv[3], &endptr);
  if (*endptr) {
     printf("spatny polomer/velikost strany\n");
     goto Usage;
  }

  switch (n) {
  case 0:
    objem = valec_objem(r, h); povrch = valec_povrch(r, h);
    printf("Valec s vyskou %g a polomerem podstavy %g ma povrch %g a objem %g\n", h, r, povrch, objem);
    break;
         
  case 1://nobreak!
  case 2: goto Usage;
  case 3:
  case 4:
  case 6:
  default:
    objem = hranol_objem(n, r, h); povrch = hranol_povrch(n, r, h);
    printf("%ld-uhelnik s vyskou %g a stranou podstavy %g ma povrch %g a objem %g\n", n, h, r, povrch, objem);
    ;
  }
  exit(EXIT_SUCCESS);
}

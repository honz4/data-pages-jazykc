/** @file
ěščřžýáíé

****** Sbírka_úloh_z_jazyka_C ******
Procvičení_učiva_II
Katedra informatiky, Univerzity Palackého v_Olomouci

****** Hledání nejdelších slov ******
Téma: Procvičení učiva II
Procvičované učivo: práce s textovým souborem, celková koncepce programu,
vícerozměrná pole, dynamická práce s pamětí, strukturované datové typy

Napište v jazyku C program, který v daném textovém souboru vyhledá zadaný počet nejdelších slov.

Název vstupního souboru a požadovaný počet hledaných
nejdelších slov by mělo být možné specifikovat z příkazové řádky při spuštění programu.

Nalezená nejdelší slova vypište pro jednoduchost na obrazovku,
přičemž slova se mohou opakovat (pokud se ve zpracovávaném textovém souboru vyskytují vícekrát),
slova stejné délky dále třiďte podle pořadí jejich výskytu v textu.

Příklad použití:
./nejdelsi_slova vstup.txt 10 (OS Linux)
nejdelsi_slova.exe vstup.txt 10 (OS Windows)

Příklad vstupního souboru:
Prijde informatik na prijimacky na ekonomku a tam se ho ptaji:
- "Tak teda kolik je 5 + 3?"
- "8, pohotove odpovi."
- "Spravne. Dame tezsi priklad. Kolik je 7 + 4?"
- "11."
- "Vyborne. A ted nejtezsi úloha. Kolik je 2 - 3?"
- "255."

Proc si informatici pletou Halloween a Vanoce?
Protoze OCT 31 je to same co DEC 25.

Lide se deli do 10 skupin. Jedni dvojkovou soustavu znaji a druzi ne.

Lide se deli do 10 skupin. Jedni znaji dvojkovou a trojkovou soustavu,
druzi neznaji ani jednu a treti si mysleli, ze tohle je vtip o dvojkove soustave.
Příklad výstupu:
informatici
informatik
prijimacky
Halloween
dvojkovou
dvojkovou
trojkovou
ekonomku
pohotove
nejtezsi
Povolené knihovny: stdio.h, stdlib.h, string.h, ctype.h
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* neco nad ramec, umime UTF-8 */
#include <wctype.h> /*iswaplha() */
#include <wchar.h>  /*mbstate_t  pro mbrlen() */
#include <locale.h> /*selocale() */
#ifdef __cplusplus__
#error !!! POUZIJTE PROSIM C kompilator, ne C++ !!!
#endif

//u malych programu, obsahujicich pouze main(), je pouziti globalu OK (misto lokalu v main(){})
char *infile; //< 1. parametr: jmeno souboru, pokud "-" pak standardni vstup
int   nwords; //< 2. velikost zebricku dlouhych slov, default 10


#define WMAX 1000 //< maximalni delka jednoho slova
char word[WMAX+1] = {'\0'}; //<buffer na slovo
int  wordbytelen; //<delka slova, v bytech
int  wordlen; //<delka slova, ve znacich

//todo: nebo pouzit https://github.com/antirez/sds ?

//todo: pouzit jako zebricek linkovany seznam? urcite delky? simple/double?

/** knihovna pro seznamy, makro "template" + static inline. */
#include "up/list.h"

/**polozka pro ulozeni slova do zebricku, ulozime si délky řetezce, at nevolame pořád strlen(), ktere je O(n).
 * Hmm, a stejně musíme "řešit" délku ve znacích a bajtech!
 */
typedef struct Word {
  int   len;     //<delka ve znacich
  int   bytelen; //<delka v bajtech
  char *str;     //<strdup()ed '\0' retezec (multi-byte/utf-8)
  struct up_list list;//zebricek
} Word;

/** spocte delku multibyte stringu ve znacich, pomoci mbrlen(). */
static
size_t mbstrlen(char *str, size_t n) {
  size_t len = 0, consumed;
  mbstate_t state;
  while (n>0) {
    consumed = mbrlen(str, n, &state);
    if (consumed <= 0)
       break;//na chyby prdime ;)
    len++;
    str += consumed; n -= consumed;
  }
  return len;
} 

/** alokuje nove slovo, ktere se da retezit pomoci up_list.
 * @param[in] str retezec, kopie pomoci strdup()
 * @param[in] bytelen delka retezce, pokud <0, spocita se pomocí strlen()
 *
 * pouzit mblen() nebo mbrlen()?
 * @note mblen() zpracuje jeden siroky znak!!! => použít mbstrlen()
 *
 * @return ukazatel na novou strukturu Word
 * @todo recyklovat pomoci free listu?
 */
static Word *new_word(char *str, int bytelen) {
  Word *pword;
  pword = malloc(sizeof *pword);
  pword->bytelen = (bytelen < 0) ? bytelen = strlen(str) : bytelen;
  pword->len = mbstrlen(str, bytelen);
  pword->str = strdup(str); //strndup() neni v stdlib,
  up_list_init(&pword->list);
  return pword;
}

/** uvolni slovo.
 */
static void word_free(Word *pword) {
  free(pword->str);
//o vypojení pword->list se postara volajici?
  free(pword);
}

UP_DEFLIST(zebricek);
int zebricek_size = 0;

void zebricek_print() {
  struct up_list *item;
  int i = 0;
  up_list_foreach(item, &zebricek) {
  //if (i > nwords+2) break;
    printf("%d:\"%s\"%p,\n", i++, container_of(item, Word,list)->str,item);
  } ;putchar('\n');
}

int main(int argc, char *argv[]) {
  int c, c2; //dva bajty vstupu
  wchar_t wch;
  char    ch2[3]={'\0','\0','\0'};//na konverzi mbstowcs()
  int inword;//stav, 0:mimo slovo, 1: ve slove
  int i;
  struct up_list *item;
  Word *pword = NULL;
  int utf8mode = 0;//UTF-8 data odhadujeme

//parametry programu:
  infile = argc > 1 ? argv[1] : __FILE__;//"-";
  nwords = argc > 2 ? atoi(argv[2]) : 10;

//s utf-8 se getchar() nevyporada!!! odhadneme UTF-8:
  if (strstr(setlocale(LC_ALL, ""), "UTF-8")) {
     utf8mode = 1;
  }

//pripadne si pře-otevřeme vstupni soubor na stdin:
  if (*infile && *infile != '-') {
   //printf("infile=%s\n",infile);
     stdin = freopen(infile, "rt", stdin);
     if (!stdin) { perror(infile); exit(EXIT_FAILURE); }
  }
  
//FILE* stream maji buffer a cteni je rychle i znak po znaku (bajt po bajtu):
  inword = 0; wordlen = wordbytelen = 0;
  while (EOF != (c=getchar())) {
  //umíme docela slušně obsloužit i multi-byte/UTF-8
    wch = 0;
    if (utf8mode && (c > 0xc0)) {
       c2 = getchar(); //nacteme dalsi znak/bajt!
       ch2[0] = c; ch2[1] = c2;
       mbstowcs(&wch, ch2, 1);//multi-byte-string to wide-char-string
//     wch = ((c & 0x3f) << 6) + (c2 & 0x1f); //nebo si to spocitame sami
//     printf("%02x %02x: wide=%x iswalpha('%s')=%d\n", ch2[0], ch2[1], wch, ch2, iswalpha(wch));
    }
//printf("%02x,",c); if (wch) {printf("%02x,",c2);} putchar('\n'); continue;//hexa vypis vstupu
    if (wch ? iswalpha(wch) : isalpha(c)) {
       if (inword) {
         //printf("++'%c' %2x\n", c, c);
           wordlen++;
           word[wordbytelen++] = c;
           if (wch) word[wordbytelen++] = c2;//2byte UTF
       } else {
         //printf("->in:word[0]='%c' %2x\n",c,c);
           inword = wordlen = 1;
           wordbytelen = 0;//zahajime sber dalsiho slova
           word[wordbytelen++] = c;
           if (wch) word[wordbytelen++] = c2;//2byte UTF
       }
//     printf("%d:word=\"%s\"[%d]\n",inword,word,wordbytelen);
    } else {//neni pismeno
//printf("noalpha: %c %02x\n", c, c);//Hmm, vstup mame utf-8!!!
       if (inword) {
           inword=0;
           word[wordbytelen] = '\0';//konec slova, do zebricku?
         //printf("word=\"%s\"[%d/%d]\n", word, wordlen, wordbytelen);
         //hledame v zebricku polozku, ktera je kratsi
           i=0;
           up_list_foreach(item, &zebricek) {
             if (i >= nwords)
                break;
             pword = container_of(item, Word,list);
             //duplicita slov!!! neni v zadani
             //je polozka kratsi nez nove slovo? vlozime pred ni
             //pocitat, porovnavat a ukladat delku ve znacich!
             if (pword->len < wordlen /*NELZE, utf: pword->bytelen < wordbytelen */) {
             // printf("\"%s\"[%d/%d] < %d\n",pword->str,pword->len,pword->bytelen, wordlen);
                break;
             }
             i++;
           }
//printf("i=%2d %p notfound=%d word=\"%s\"\n", i,pword,  &zebricek==item, word);
           if (i < nwords) {
              pword = new_word(word, wordbytelen);
              if (&zebricek == item) {
                //prosli jsme celý zebricek a nic, lze jeste pripojit na konec:
                  up_list_prepend(&zebricek, &(pword->list));
                  zebricek_size++;
                //printf("\"%s\" append\n",pword->str);
              }
              else {
                  up_list_prepend(item, &(pword->list));
                //printf("\"%s\" prepend %d!!! \"%s\"%p\n", pword->str,i, container_of(item, Word,list)->str,item);
                  //polozku co vypadla ze zebricku vyradime:
                  if (zebricek_size >= nwords) {
                      pword = container_of(up_list_remove(up_list_last(&zebricek)), Word,list);
                      word_free(pword);
                  }
                  else {
                      zebricek_size++;
                  }
              }
            //zebricek_print();
           }
       }
       //nejsme ve slove: nedelame nic
    }
  }//while !EOF
//nakonec vypiseme zebricek, muzeme zebricek_print(), ale pocvicime up_list:
  i = 1;
  up_list_foreach(item, &zebricek) {
    pword = container_of(item, Word,list);
    printf("%2d. %s[%d]\n", i++, pword->str, pword->len);
  }
  return EXIT_SUCCESS;
}

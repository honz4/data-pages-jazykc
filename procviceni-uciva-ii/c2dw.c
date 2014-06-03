#include <stdio.h>
#include <stdlib.h>
///konvertuje takoveto bloky na dokuwiki .txt
///dalsi radek

#define LINESIZE 1000
char linebuf [LINESIZE+1];
char linebuf2[LINESIZE+1];
char ws[LINESIZE+1];

int main(int argc, char *argv[]) {
  char *infile;
  int doku;
  char *pch;
  int slash, skip;//state, pocitadlo lomitek

  infile = argc > 1 ? argv[1] : __FILE__;//testujeme sami na sobe

  stdin = freopen(infile, "r", stdin);//todo: kontrola NULL?

  ///dalsi dokuwiki
  ///druhy radek
  ///treti radek
  ////ctvrty radek, ale dokuwiki podle toho 'neradkuje'!
  printf("======%s======\n<%code c>\n", infile, 'c');
  while (1) {
    pch = fgets(linebuf, LINESIZE, stdin);
    if (!pch)
       break;
    slash = sscanf(linebuf, " //%[/]%n%s", ws, &skip, linebuf2);
//  printf("%2d:%p:%s", slash, pch,slash>0 && skip ? linebuf+skip : linebuf);
    if (slash==2) {
       if (doku) {
          printf("%s", linebuf+skip);
       } else {
          doku=1;
          printf("</co" "de>\n%s", linebuf+skip);
       }
    } else {
       if (doku) {
          printf("<co" "de c>\n%s", linebuf);
          doku=0;
       } else {
          printf("%s", linebuf);
       }
    }
  }
  printf("</co" "de>\n");

  return EXIT_SUCCESS;
}

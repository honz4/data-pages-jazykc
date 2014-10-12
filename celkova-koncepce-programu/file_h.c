/** @file
 * pomucka/utilita na vytvoreni mustru file.h souboru (include guard, C++/cdecl guard)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* pomucka/utilita na vytvoreni mustru file.h souboru
 *
 * umistit do ${HOME}/bin, pomoci make install, viz Makefile
 */
int main(int argc, char *argv[]) {
  char *FILE_H, *file_h;
  int i;
  if (argc <= 1) {
     printf("Usage: %s file.h\n", argv[0]);
     exit(1);//false, nebo return 1;
  }

  file_h = argv[1];
//kopie
  FILE_H = strdup(file_h);
//alfanumericke znaky velkymi pismeny, vse ostatni nahradime '_'
//to, ze FILE_H[0] neni alfa neresime
  for (i=0; file_h[i]; i++) {
      FILE_H[i] = (isalnum(file_h[i])) ? toupper(file_h[i]) : '_';
  }
  printf("#ifndef %s_INCLUDED\n#define %s_INCLUDED\n#endif\n\n#ifdef __cplusplus\nextern \"C\" {\n#endif\n", FILE_H, FILE_H);
//nasl. je citelnejsi (pomoci concat string literalu):
  printf("\n#ifdef __cplusplus\n"
         "}\n"
	 "#endif\n#undef %s_INCLUDED\n", FILE_H);
  return 0;
}

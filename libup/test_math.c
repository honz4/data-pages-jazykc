#include "up/math.h"

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
  printf("%i %li %lli\n", ipow(-2,10), ipowl(-2,30), ipowll(-2,63));//OK?
  printf("%i %li %lli\n", ipow(2,10),  ipowl(2,30),  ipowll(2,63));
  printf("%i %li %lli\n", ipow(2,-10),  ipowl(2,-30),  ipowll(2,-63));//blbost
  return EXIT_SUCCESS;
}

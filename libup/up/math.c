#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "up/math.h"

/** celociselna mocnina, integer.
 *
 * http://stackoverflow.com/questions/101439/the-most-efficient-way-to-implement-an-integer-based-power-function-powint-int
 * http://en.wikipedia.org/wiki/Exponentiation_by_squaring
 *
 * hmm, funguje jen pro exp > 0 ?
 */
int ipow (int base, int exp) {
    int result = 1;
    while (exp) {
      if (exp & 1) result *= base;
      exp >>= 1;
      base *= base;
    }
    return result;
}

/** celociselna mocnina, long.
 */
long ipowl (long base, long exp) {
    long result = 1L;
    while (exp) {
      if (exp & 1) result *= base;
      exp >>= 1;
      base *= base;
    }
    return result;
}

long long ipowll (long long base, long long exp) {
    long long result = 1LL;
    while (exp) {
      if (exp & 1) result *= base;
      exp >>= 1;
      base *= base;
    }
    return result;
}


#ifdef __cplusplus
}
#endif
#undef MATH_H_INCLUDED

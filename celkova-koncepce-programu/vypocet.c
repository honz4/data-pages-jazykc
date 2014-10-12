#include "vypocet.h"
#include <math.h>

double valec_objem(double r, double h) {
  return M_PI * r*r * h; 
}

double valec_povrch(double r, double h) {
  double obvod = 2.0 * M_PI * r;
  return   obvod * r //dve podstavy
         + obvod * h; //plast
}

#define NUHEL(n,s) ((n)/4.0 * (s)*(s) * cos(M_PI/(n))/sin(M_PI/(n)))

/* n-uhelnik: n/4 s^2 cotg PI/n
 *
 * cotg = cos/sin
 */
double hranol_objem(long n, double s, double h) {
  return s*s * 0.25*n * cos(M_PI/n)/sin(M_PI/n) * h;
}

double hranol_povrch(long n, double s, double h) {
  return  n/2.0 * s*s * cos(M_PI/n)/sin(M_PI/n)//dve podstavy
         + s*n * h; //plast
}

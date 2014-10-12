#ifndef VYPOCET_H_INCLUDED
#define VYPOCET_H_INCLUDED
#endif
//double include guard a extern "C" kvuli pouziti v C++ pomoci file_h.c
#ifdef __cplusplus
extern "C" {
#endif

double valec_objem (double r, double h);
double valec_povrch(double r, double h);

/*nebo jako makra pomoci hranol_objem()?
 */
double hranol3_objem (double s, double h);
double hranol3_povrch(double s, double h);

double hranol4_objem (double s, double h);
double hranol4_povrch(double s, double h);

double hranol6_objem (double s, double h);
double hranol6_povrch(double s, double h);

/* pravidelny n-uhelnik: n/4 * s^2 * cotg pi/n
 * 3-uhelnik sqrt(3)/4
 * 6-uhelnik 1.5 sqrt(3
 */ 
double hranol_objem (long n, double s, double h);
double hranol_povrch(long n, double s, double h);


#ifdef __cplusplus
}
#endif
#undef VYPOCET_H_INCLUDED

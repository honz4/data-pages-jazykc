/** @file
 */
#ifndef UP_PORTABLE_H_INCLUDED
#define UP_PORTABLE_H_INCLUDED
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*vetsina hlavickovych souboru C standard knihovny: */
#include <stddef.h>
#include <stdint.h>

/** zjisti ofset clena uvnitr struktury, zpravidla #define-ovano uz v stdder.h */
#ifndef offsetof
#define offsetof(type,field) ((unsigned long)&(((type*)0)->field))
#endif

/** vraci ukazatel na zacatek struct type, jejimz clenem je field. */
#ifndef container_of
#define container_of(ptr, type, field) ((type*)((char*)(ptr) - offsetof(type,field)))
#endif

#ifndef alignof
#define alignof(type) offsetof(struct{char c; type t;}, t)
#endif

#ifndef ARRAY_NELEM
#define ARRAY_NELEM(a) (sizeof(a)/sizeof((a)[0]))
#endif

#ifdef __cplusplus
}
#endif
#undef UP_PORTABLE_H_INCLUDED
